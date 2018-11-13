from pprint import pprint
from itertools import chain, compress
from functools import reduce, partial
from glob import iglob
from yaml import safe_load
from numba import jit
from cytoolz import compose
from pyspark.sql import SparkSession, Row, DataFrame, functions as f
from pyspark.sql.types import ArrayType, BooleanType
from dltools import SpkHits, load_combiner
from dltools.sacla import restructure, load_analyzer


# %% Load config file
with open("sacla_analysis-config.yaml", "r") as file:
    print("Loading config file...")
    config = safe_load(file)
pprint(config)


# %% Load momentum model
print("Loading momentum model...")
analyzer = load_analyzer(config["momentum_analyzer"])
print(analyzer)


# %% Load PySpark
print("Loading PySpark...")
builder = (SparkSession
           .builder
           .config("spark.jars.packages",
                   "org.diana-hep:spark-root_2.11:0.1.15,"
                   "org.mongodb.spark:mongo-spark-connector_2.11:2.3.1")
           )
with builder.getOrCreate() as spark:
    print(spark)

    # %% Load data
    print("Loading data...")
    globbed = chain.from_iterable(iglob(patt) for patt in config["target_files"])
    loadme = (spark.read.format("org.dianahep.sparkroot").load(f) for f in sorted(set(globbed)))
    df = restructure(reduce(DataFrame.union, loadme))
    df.printSchema()
    df.show()
    print(
        df
            .select(f.explode("hits").alias("h"))
            .select(f.col("h.t").alias("t"),
                    f.col("h.x").alias("x"),
                    f.col("h.y").alias("y"),
                    f.col("h.flag").alias("flag"))
            .limit(20)
            .toPandas()
    )

    # %% Analyze momentum
    print("Analyzing momentum...")

    @jit(nopython=True, nogil=True)
    def isgood(flag):
        return flag <= 6

    wheregood = f.udf(compose(list, partial(map, isgood)), ArrayType(BooleanType()))
    hitfilter = f.udf(compose(list, partial(map, Row.asDict), compress), SpkHits)
    analyzed = (
        df
            .withColumn("filtered", hitfilter("hits", wheregood("hits.flag")))
            .withColumn("analyzed", analyzer("filtered"))
    )
    analyzed.printSchema()
    analyzed.show()
    print(
        analyzed
            .select(f.explode("analyzed").alias("h"))
            .select(f.explode("h.as_").alias("as_", "m"))
            .select("as_", "m.*")
            .limit(20)
            .toPandas()
    )

    # %% Combine hits
    print("Combining hits...")
    combiner = load_combiner(r=2, white_list=[["particle_b", "particle_a"]], allow_various=False, allow_dup=False)
    combined = analyzed.withColumn("combined", combiner("analyzed"))
    combined.printSchema()
    combined.show()
    print(
        combined
            .select(f.explode("combined.as_").alias("h"))
            .select(f.explode("h").alias("as_", "m"))
            .select("as_", "m.*")
            .limit(20)
            .toPandas()
    )

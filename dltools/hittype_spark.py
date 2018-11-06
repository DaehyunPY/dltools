from pyspark.sql.types import (
    StructType, StructField,
    ArrayType, MapType,
    IntegerType, DoubleType, StringType,
)

__all__ = [
    'SpkAnalyzedHit',
    'SpkHit',
    'SpkHits',
]

SpkAnalyzedHit = StructType([
    StructField('pz', DoubleType(), nullable=False),
    StructField('px', DoubleType(), nullable=False),
    StructField('py', DoubleType(), nullable=False),
    StructField('ke', DoubleType(), nullable=False),
])

SpkHit = StructType([
    StructField('t', DoubleType(), nullable=False),
    StructField('x', DoubleType(), nullable=False),
    StructField('y', DoubleType(), nullable=False),
    StructField('as', MapType(StringType(), SpkAnalyzedHit), nullable=True),
    StructField('flag', IntegerType(), nullable=True),
])

SpkHits = ArrayType(SpkHit)

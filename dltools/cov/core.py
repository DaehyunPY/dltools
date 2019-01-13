import typing

import pyspark
from numba import jit
import numpy as np


__all__ = [
    "digitize", "increase",
    "SpkPersist", "SpkRepart", "SpkCoalesce", "SpkMapPartThanSum",
    "ReferTo", "AppendCov", "AppendCCov",
]


def digitize(
        arr: (typing.List[float], np.ndarray),
        bins: (typing.List[float], np.ndarray),
        ) -> dict:
    n, m = len(arr), len(bins)
    digitized = np.digitize(arr, bins=bins)
    where = (0 < digitized) & (digitized < m)
    return {
        "digitized": digitized,
        "where": where,
    }


@jit(nopython=True, nogil=True)
def increase(counter: np.ndarray, at: tuple) -> np.ndarray:
    counter[at] += 1
    return counter


class SpkPersist:
    def __init__(self, on: bool = True, opt: str = "MEMORY_AND_DISK"):
        options = {
            "DISK_ONLY": pyspark.StorageLevel.DISK_ONLY,
            "DISK_ONLY_2": pyspark.StorageLevel.DISK_ONLY_2,
            "MEMORY_ONLY": pyspark.StorageLevel.MEMORY_ONLY,
            "MEMORY_ONLY_2": pyspark.StorageLevel.MEMORY_ONLY_2,
            "MEMORY_AND_DISK": pyspark.StorageLevel.MEMORY_AND_DISK,
            "MEMORY_AND_DISK_2": pyspark.StorageLevel.MEMORY_AND_DISK_2,
            "OFF_HEAP": pyspark.StorageLevel.OFF_HEAP,
        }

        if opt not in options:
            raise ValueError(f'Argument "opt" has to be one of {", ".join(options)}!')

        self.__on = on
        self.__opt = options[opt]

    def __call__(self, df: pyspark.sql.DataFrame) -> pyspark.sql.DataFrame:
        if self.__on:
            return df.persist(self.__opt)
        return df

    def __ror__(self, other: pyspark.sql.DataFrame) -> pyspark.sql.DataFrame:
        return self(other)


class SpkRepart:
    def __init__(self, npart: typing.Optional[int] = None):
        self.__npart = npart

    def __call__(self, df: pyspark.sql.DataFrame) -> pyspark.sql.DataFrame:
        npart = self.__npart
        if npart is None:
            return df
        return df.repartition(npart)

    def __ror__(self, other: pyspark.sql.DataFrame) -> pyspark.sql.DataFrame:
        return self(other)


class SpkCoalesce:
    def __init__(self, npart: typing.Optional[int] = None):
        self.__npart = npart

    def __call__(self, df: pyspark.sql.DataFrame) -> pyspark.sql.DataFrame:
        npart = self.__npart
        if npart is None:
            return df
        return df.coalesce(npart)

    def __ror__(self, other: pyspark.sql.DataFrame) -> pyspark.sql.DataFrame:
        return self(other)


Return = typing.TypeVar("Return")


class SpkMapPartThanSum(typing.Generic[Return]):
    def __init__(self, f: typing.Callable[[typing.List[pyspark.sql.Row]], Return]):
        self.__f = f

    def __call__(self, df: pyspark.sql.DataFrame) -> Return:
        return df.rdd.mapPartitions(self.__f).sum()

    def __ror__(self, other: pyspark.sql.DataFrame) -> Return:
        return self(other)


class ReferTo:
    def __init__(self, ref: str, *keys: str):
        self.__ref, self.__keys = ref, keys

    def __call__(self, d: dict) -> dict:
        ref, keys = self.__ref, self.__keys
        return {
            **{k: d[ref] for k in keys},
            **d,
        }

    def __ror__(self, other: dict) -> dict:
        return self(other)


class AppendCov:
    def __init__(self, *keys: str):
        if not 1 < len(keys) < 4:
            raise ValueError(f"Too less or many keys: {', '.join(keys)}!")

        self.__keys = keys

    def __call__(self, d: dict) -> dict:
        keys = self.__keys

        if len(keys) == 2:
            i, j = keys
            return {
                f"Cov[{i},{j}]": (
                    d[f"Sum[{i}{j}]"] / d["N"]
                    - np.einsum("i...,j->i...j",
                                d[f"Sum[{i}]"],
                                d[f"Sum[{j}]"]) / d["N"] ** 2
                ),
                **d,
            }

        if len(keys) == 3:
            i, j, k = keys
            return {
                f"Cov[{i},{j},{k}]": (
                    d[f"Sum[{i}{j}{k}]"] / d["N"]
                    - np.einsum("i...j,k->i...jk",
                                d[f"Sum[{i}{j}]"],
                                d[f"Sum[{k}]"]) / d["N"] ** 2
                    - np.einsum("i...,jk->i...jk",
                                d[f"Sum[{i}]"],
                                d[f"Cov[{j},{k}]"]) / d["N"]
                    - np.einsum("j...,ik->ij...k",
                                d[f"Sum[{j}]"],
                                d[f"Cov[{i},{k}]"]) / d["N"]
                ),
                **d,
            }

    def __ror__(self, other: dict) -> dict:
        return self(other)


class AppendCCov:
    def __init__(self, *keys: str):
        if not 1 < len(keys) < 4:
            raise ValueError(f"Too less or many keys: {', '.join(keys)}!")

        self.__keys = keys

    def __call__(self, d: dict) -> dict:
        keys = self.__keys

        if len(keys) == 2:
            i, j = keys
            return {
                f"Cov[{i},{j}]": (
                    d[f"Sum[{i}{j}]"] / d["N"]
                    - d[f"Sum[{i}]Sum[{j}]"] / d["N"] ** 2
                ),
                **d,
            }

        if len(keys) == 3:
            i, j, k = keys
            return {
                f"Cov[{i},{j},{k}]": (
                    d[f"Sum[{i}{j}{k}]"] / d["N"]
                    - d[f"Sum[{i}{j}]Sum[{k}]"] / d["N"] ** 2
                    - d[f"Sum[{i}{k}]Sum[{j}]"] / d["N"] ** 2
                    - d[f"Sum[{j}{k}]Sum[{i}]"] / d["N"] ** 2
                    + 2 * d[f"Sum[{i}]Sum[{j}]Sum[{k}]"] / d["N"] ** 3
                ),
                **d,
            }

    def __ror__(self, other: dict) -> dict:
        return self(other)

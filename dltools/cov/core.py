import typing

from numba import jit
import numpy as np


__all__ = [
    "digitize", "increase", "AppendCov", "ReferTo",
]


def digitize(
        arr: (typing.List[float], np.ndarray),
        bins: (typing.List[float], np.ndarray),
        ) -> dict:
    n, m = len(arr), len(bins)
    digitized = np.digitize(arr, bins=bins)
    where = (0 < digitized) & (digitized < m)
    return {
        "args": np.arange(n)[where],
        "digitized": digitized[where],
    }


@jit(nopython=True, nogil=True)
def increase(counter: np.ndarray, at: tuple) -> np.ndarray:
    counter[at] += 1
    return counter


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

#!/bin/bash

dir=$(realpath "$(dirname $0)")

docker run \
    --rm \
    --interactive \
    --volume "$dir":/app \
    --workdir /app \
    quay.io/pypa/manylinux1_x86_64 \
    bash <<EOF

for PY in /opt/python/cp37-cp37m/bin/python \
          /opt/python/cp36-cp36m/bin/python \
          /opt/python/cp35-cp35m/bin/python
do
    \$PY -m pip install pybind11 cython numpy
    \$PY setup.py sdist bdist_wheel
done
ls dist/*.whl | xargs -i auditwheel repair {}

EOF

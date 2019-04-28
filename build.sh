#!/bin/bash
dir="$(pwd)"

docker run \
	--rm \
	--interactive \
	--volume "$dir":/app \
	--workdir /app \
	quay.io/pypa/manylinux1_x86_64 bash <<EOF
        for py in \
            /opt/python/cp37-cp37m/bin/python \
            /opt/python/cp36-cp36m/bin/python \
            /opt/python/cp35-cp35m/bin/python
        do
            \$py -m pip install pybind11 cython numpy
            \$py setup.py sdist bdist_wheel
        done
        ls dist/*.whl | xargs -i auditwheel repair {}
EOF

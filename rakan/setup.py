from distutils.core import setup, Extension
from Cython.Build import cythonize

extensions = [
    Extension(
        "rakan",
        ["cRakan/wrapper.pyx"],
        language="c++",
        extra_compile_args=["-std=c++11"],
        extra_link_args=["-std=c++11"]
    )
]

setup(
    name='MCMC Engines',
    # ext_modules=cythonize('rakan.pyx', annotate=True, gdb_debug=True),
    ext_modules=cythonize(extensions)
)
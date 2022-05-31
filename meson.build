project('my_module', 'cpp',
        default_options: ['cpp_std=c++17'])

# ===========================================================
# Python 3
# ===========================================================

# python 3 dependency
python_dep = dependency('python3')

# python 3 import for extension_module
python_module = import('python')
inst = python_module.find_installation(get_option('python_executable'))

# ===========================================================
# PyBind11
# ===========================================================

# pybind11 dependency
pybind11_dep = dependency('pybind11')

inst.extension_module('sente', 'src/module.cpp',
                      'src/Game/Group.cpp', 'src/Game/Group.h', 'src/Game/GoGame.h' 'src/Game/GoGame.cpp',

                      dependencies: [pybind11_dep, python_dep])


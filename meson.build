project('sente', 'cpp',
        default_options: ['cpp_std=c++17'], version: '0.4.4')

# ===========================================================
# Python 3
# ===========================================================

# python 3 import for extension_module
python_module = import('python')
inst = python_module.find_installation()

# ===========================================================
# PyBind11
# ===========================================================

# pybind11 dependency
pybind11_dep = dependency('pybind11', required: true)

# disable throwing std::bad_access on darwin
if host_machine.system() == 'darwin'
    # fixme: find a better way to enable std::variant usage on darwin
    args = ['-D_LIBCPP_DISABLE_AVAILABILITY']
else
    args = []
endif

inst.extension_module('sente', 'src/module.cpp',
                      'src/Game/Group.cpp', 'src/Game/Group.h', 'src/Game/GoGame.h', 'src/Game/GoGame.cpp',
                      'src/Game/Move.cpp', 'src/Game/Move.h', 'src/Game/Board.h',
                      'src/Game/Board.cpp', 'src/Utils/Tree.h', 'src/Utils/SGF/SGF.cpp', 'src/Utils/SGF/SGF.h',
                      'src/Game/GoComponents.h', 'src/Game/GoComponents.cpp',
                      'src/Utils/SenteExceptions.cpp', 'src/Utils/SenteExceptions.h',
                      'src/Game/LifeAndDeath.h', 'src/Game/LifeAndDeath.cpp',
                      'src/Utils/Numpy.h', 'src/Utils/Numpy.cpp', 'src/Utils/SGF/SGFNode.h', 'src/Utils/SGF/SGFNode.cpp',
                      'src/Utils/SGF/SGFProperty.h', 'src/Utils/SGF/SGFProperty.cpp',
                      'src/Utils/GTP/Tokens/Token.h', 'src/Utils/GTP/Tokens/Token.cpp',
                      'src/Utils/GTP/Tokens/Seperator.h', 'src/Utils/GTP/Tokens/Seperator.cpp',
                      'src/Utils/GTP/Tokens/Literal.h', 'src/Utils/GTP/Tokens/Literal.cpp',
                      'src/Utils/GTP/Parser.h', 'src/Utils/GTP/Parser.cpp',
                      'src/Utils/GTP/DefaultSession.h', 'src/Utils/GTP/DefaultSession.cpp',
                      'src/Utils/GTP/Controller.h', 'src/Utils/GTP/Controller.cpp',
                      'src/Utils/GTP/Session.h', 'src/Utils/GTP/Session.cpp',
                      'src/Utils/GTP/PythonBindings.cpp', 'src/Utils/GTP/PythonBindings.h',
                      dependencies: [pybind11_dep, inst.dependency()],
                      cpp_args: args)


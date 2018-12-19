# -*- mode: python -*-
import sys, os, os.path, platform, re

# Inherit the environment from my parent.
Import('env')

# Make a copy of the environment, so my changes are limited in this directory and sub-directories.
env = env.Clone()

#global variables define

env['topdir'] = env.Dir('#').abspath
env['builddir'] = env['topdir'] + '/' + env['BUILD_DIR'][1:]
env['relative_build_dir'] = 'build/' + env['mode'] + env['target']

#sustitute variable in test.h
env.Append(ACSUBST = {'BUILDDIR': env['relative_build_dir']})
env.Append(ACSUBST = {'TOP_BUILDDIR': env['builddir']})
env.Append(ACSUBST = {'TOP_SRCDIR': env['topdir']})
env.Append(ACSUBST = {'abs_top_srcdir': env['topdir']})
env.Append(ACSUBST = {'TOP_DIR': env['TOP_DIR']})
env.Append(ACSUBST = {'BUILD_DIR': env['BUILD_DIR']})

env['LIBPATH'] = [
                  env['TOP_DIR'] + '/lib/',
                  env['TOP_DIR'] + '/_external/usr/local/lib64/',
                 ]

#env.aCheckLibrary('anet')

#define compile options
env.Prepend(CPPPATH=['#'])
env.Append(CPPPATH = [
                      env['BUILD_DIR'],
                      env['BUILD_DIR'] + '/_external/usr/local/include/nlu_processer',
                      env['BUILD_DIR'] + '/_external/usr/local/include/crf',
                      env['BUILD_DIR'] + '/_external/usr/local/include/alog',
                      env['BUILD_DIR'] + '/_external/usr/local/include/autil',
                      env['BUILD_DIR'] + '/_external/usr/local/include/nlu_reg',
                      env['BUILD_DIR'] + '/_external/usr/local/include/proto',
                      env['BUILD_DIR'] + '/_external/usr/local/include/libconfig',
                      env['BUILD_DIR'] + '/_external/usr/local/include/tinyxml',
                      '/usr/local/include',
                     ]
          )

env['LIBPATH'] = [
                  env['TOP_DIR'],
                  env['TOP_DIR'] + '/lib/',
                  env['TOP_DIR'] + '/_external/usr/local/lib64/',
                  env['TOP_DIR'] + '/_external/usr/local/lib/',
                  env['TOP_DIR'] + '/_external/usr/lib64/',
                 ]


env['LIBS'] = ['alog', 'autil', 'nlu_reg','tiny', 'libconfig++', 'proto_def']

env.AppendENVPath('LD_LIBRARY_PATH', '/lib64:/usr/local/lib64:/usr/local/lib:' + Dir(env['LIB_DIR']).abspath)

env['LINKFLAGS'] = []

version='0.1.10'
version_flags = {'CCFLAGS':'-DVERSION="\\""' + version + '"\\""'}
env.MergeFlags(version_flags)

loggerconf = env['TOP_DIR'] + '/test/alog.conf'
env.Append(ACSUBST = {'DOTEST_LOGGER_CONF': loggerconf})
env.aACSubst(target = ['test/test.h'],
             source = ['test/test.h.in'])

#add sub directory.

env.aCompileProto("proto/qp.proto")
env.aCompileProto("proto/nlu_dm.proto")
env.aCompileProto("proto/classifier.proto")
qp_proto = ["proto/qp.pb.cc"]
nlu_dm_proto = ["proto/nlu_dm.pb.cc"]
classifier_proto = ["proto/classifier.pb.cc"]
env.aDirs('proto')

pattern_src = env.aDirs('pattern')
wordseg_src = env.aDirs('wordseg')
crf_src = env.aDirs("crf_model")
dict_matcher_src = env.aDirs("dict_matcher")
classifier_src = env.aDirs('classifier')
post_processer_src = env.aDirs('post_processer')
nlu_processer_src = env.aDirs("nlu_processer")
nlu_test_src = env.aDirs("nlu_test")
all_cpps = [qp_proto, nlu_dm_proto, classifier_proto, wordseg_src, pattern_src, crf_src, dict_matcher_src, classifier_src, post_processer_src, nlu_processer_src]
env.aLibrary(target = 'nlu',
             source = all_cpps,
             )
env.aDirs('main')

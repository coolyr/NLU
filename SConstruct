# -*- mode: python -*-
import sys, os, os.path, platform, re, time

os.environ['build_options'] = '''
heapchecktype = 'tcmalloc'
enable_strip = 'False'
'''

env = Environment(tools=['default', 'ascons'])

env['version'] = '0.0.1'
env['prjName'] = 'nlu'

env.SConscript(
    dirs='.',
    exports='env',
    variant_dir=env['BUILD_DIR'],
    duplicate=1
    )

env.aInstall(env['install_prefix'] + '/proto/', env['TOP_DIR'] + '/build/release64/proto/*.h')
env.aInstall(env['install_prefix'] + '/proto/', env['TOP_DIR'] + '/build/release64/proto/*.proto')

#env.aCheckStyle('pattern')
nlu_includes = ['nlu_processer/nlu_processer.h']
env.aInstallHeader(nlu_includes, '', True)

#proto_includes = ['proto/*.h']
#env.aInstallHeader(proto_includes, '', True)

env.aInstall('proto', 'build/release64/proto/*.h')
env.aInstall(env['install_includedir'] + '/proto/', 'build/release64/proto/*.h')
env.aInstall(env['install_libdir'], 'build/release64/libnlu.so')
env.aInstall(env['install_libdir'], 'lib/*')

releaseStr = 'nightly_' + time.strftime("%Y%m%d%H%M", time.localtime())

if env['mode'] == 'release':
    releaseStr = 'rc_2'

env.aRpm(
    FILES = [
             env['install_libdir'],
            ],
    NAME = env['prjName'],
    VERSION = env['version'],
    RELEASE = releaseStr,
    REQUIRES = [
               ],
    GROUP = 'Applications',
    SUMMARY = '%{name}')

env.aRpm(PACKAGE_NAME = 'devel',
         NAME = env['prjName'],
         FILES = [
                  env['install_includedir'],
                  env['install_libdir'],
                  env['install_prefix'] + '/proto' ,
                 ],
         SUMMARY = 'Header files, libraries and development documentation for %{name}.',
         REQUIRES = [
                    ],
         GROUP = '%{group}',
         DESCRIPTION = 'This package contains the header files, libraries.\n'
                       'If you like to develop programs using %{name}, \n'
                       'you will need to install %{name}-devel.'
        )


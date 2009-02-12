# -*- Python -*-

freeorion_version = 'v0.3.12'

gigi_version = '0.6.0'

ft_pkgconfig_version = '9.0.0'
ft_version = '2.1.2'
ft_win32_lib_name = 'freetype214MT'

ogre_version = '1.6.1'
ois_version = '1.2.0'

boost_version_string = '1.36'
def BoostStringToNumber(version_string):
    pieces = version_string.split('.')
    return str(int(pieces[0]) * 100000 + int(pieces[1]) * 100 + (3 <= len(pieces) and int(pieces[2]) or 0))
boost_version = BoostStringToNumber(boost_version_string)

bullet_version = '2.73'

openal_pkgconfig_version = '0.0.8'
alut_pkgconfig_version ='1.1.0'
openal_win32_lib_name = 'OpenAL32'

ogg_pkgconfig_version = '1.1.3'
vorbis_pkgconfig_version = '1.1.2'

graphviz_pkgconfig_version = '2.8'

python_win32_libname = 'python25'

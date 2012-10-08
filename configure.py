import sys, os, optparse, subprocess

# Avoid .pyc files
sys.dont_write_bytecode = True

# Auxilary
def toPosix(path):
    if os.sep != "/" and os.sep in path:
        return path.replace(os.sep, "/")
    return path

# Parse command line
parser = optparse.OptionParser(usage='usage: %prog [options] settings')
parser.add_option('--build-type', dest='build_type', choices=['debug', 'release'], default='release', help='build type: debug or release [default: %default]', metavar='<type>')
parser.add_option('--build-dir', dest="build_dir", help='build directory [default: current directory]', metavar='<directory>')
parser.add_option('--source-dir', dest="source_dir", help='sources directory [default: script directory]', metavar='<directory>')
parser.add_option('--qt', dest="qt_dir", help='qt directory', metavar='<directory>')
parser.add_option('--boost', dest="boost_dir", help='boost directory', metavar='<directory>')
parser.add_option('--ffmpeg', dest="ffmpeg_dir", help='ffmpeg directory', metavar='<directory>')
parser.add_option('--lame', dest="lame_dir", help='lame directory', metavar='<directory>')
parser.add_option('--portaudio', dest="portaudio_dir", help='portaudio directory', metavar='<directory>')
options, args = parser.parse_args()

if len(args) != 0:
    parser.error('incorrect number of arguments')
    
if options.boost_dir == None or options.ffmpeg_dir == None or options.lame_dir == None or options.portaudio_dir == None:
    parser.error('--boost, --ffmpeg, --portaudio, and --lame are required parameters')

# Initialize
current_dir = os.getcwd()
script_dir = os.path.dirname(os.path.realpath(__file__))

if options.build_dir == None:
    options.build_dir = current_dir
if options.source_dir == None:
    options.source_dir = script_dir
    
project = os.path.join(options.source_dir, 'applications.pro')
project_config = os.path.join(options.build_dir, 'config.in')
project_config_out = os.path.join(options.build_dir, 'config.pri')
project_log = os.path.join(options.build_dir, 'config.log')
qmake_cache = os.path.join(options.build_dir, '.qmake.cache')

# Clear build dir
if os.path.exists(project_config):
    os.remove(project_config)
if os.path.exists(project_log):
    os.remove(project_log)
if os.path.exists(qmake_cache):
    os.remove(qmake_cache)

# Start configure        
print 'Configuring...'

# Checking Qt
with open(project_log, 'w') as f:
    if subprocess.call([os.path.join(options.qt_dir, 'qmake'), '-v'], stderr=f) == 1:
        sys.exit('Can\'t find qmake. Aborting.')

# Generate .qmake.cache
with open(qmake_cache, 'w') as f:
    f.write('SOURCE_TREE = ' + toPosix(options.source_dir) + '\n');
    f.write('BUILD_TREE = ' + toPosix(options.build_dir) + '\n');

# Generate project config
with open(project_config, 'w') as f:
    f.write('CONFIG += ' + toPosix(options.build_type) + '\n');
    f.write('BOOST_PATH = "' + toPosix(options.boost_dir) + '"\n');
    f.write('FFMPEG_PATH = "' + toPosix(options.ffmpeg_dir) + '"\n');
    f.write('LAME_PATH = "' + toPosix(options.lame_dir) + '"\n');
    f.write('PORTAUDIO_PATH = "' + toPosix(options.portaudio_dir) + '"\n');

if os.path.exists(project_config_out):
    os.remove(project_config_out)
os.rename(project_config, project_config_out)
    
# Generate make files etc.
with open(project_log, 'a') as f:
    if subprocess.call([os.path.join(options.qt_dir, 'qmake'), '-recursive', project], stderr=f) == 1:
        sys.exit('Qmake call failed. Aborting.')

# Success configure
print 'Configure has finished. You may run make to build the project now.'

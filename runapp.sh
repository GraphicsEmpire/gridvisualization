##Bash Script File to setup environment vars and run MB App
export QTDIR=/base/qnxqt5feb2013
export QML_IMPORT_PATH=$QTDIR/imports
export QML2_IMPORT_PATH=$QTDIR/qml
export QT_PLUGIN_PATH=$QTDIR/plugins
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH

export QQNX_PHYSICAL_SCREEN_SIZE=1440,540

##Run
chmod a+x GridVisualization

##Run
path=${0%%?????????}
on /projects/GridVisualization/GridVisualization
##on /apps/com.mbrdna.MBTestQNX.testRel_a_MBTestQNX2f676b0d/native/MBTestQNX

# Script generated with Bloom
pkgdesc="ROS - The pheeno_ros package contains necessary files for run and control an individual Pheeno unit with ROS."
url='https://acslaboratory.github.io'

pkgname='ros-kinetic-pheeno-ros'
pkgver='0.1.1_3'
pkgrel=1
arch=('any')
license=('BSD'
)

makedepends=('ros-kinetic-actionlib'
'ros-kinetic-actionlib-msgs'
'ros-kinetic-catkin'
'ros-kinetic-geometry-msgs'
'ros-kinetic-roscpp'
'ros-kinetic-rospy'
'ros-kinetic-std-msgs'
)

depends=('ros-kinetic-actionlib'
'ros-kinetic-actionlib-msgs'
'ros-kinetic-geometry-msgs'
'ros-kinetic-roscpp'
'ros-kinetic-rospy'
'ros-kinetic-std-msgs'
)

conflicts=()
replaces=()

_dir=pheeno_ros
source=()
md5sums=()

prepare() {
    cp -R $startdir/pheeno_ros $srcdir/pheeno_ros
}

build() {
  # Use ROS environment variables
  source /usr/share/ros-build-tools/clear-ros-env.sh
  [ -f /opt/ros/kinetic/setup.bash ] && source /opt/ros/kinetic/setup.bash

  # Create build directory
  [ -d ${srcdir}/build ] || mkdir ${srcdir}/build
  cd ${srcdir}/build

  # Fix Python2/Python3 conflicts
  /usr/share/ros-build-tools/fix-python-scripts.sh -v 2 ${srcdir}/${_dir}

  # Build project
  cmake ${srcdir}/${_dir} \
        -DCMAKE_BUILD_TYPE=Release \
        -DCATKIN_BUILD_BINARY_PACKAGE=ON \
        -DCMAKE_INSTALL_PREFIX=/opt/ros/kinetic \
        -DPYTHON_EXECUTABLE=/usr/bin/python2 \
        -DPYTHON_INCLUDE_DIR=/usr/include/python2.7 \
        -DPYTHON_LIBRARY=/usr/lib/libpython2.7.so \
        -DPYTHON_BASENAME=-python2.7 \
        -DSETUPTOOLS_DEB_LAYOUT=OFF
  make
}

package() {
  cd "${srcdir}/build"
  make DESTDIR="${pkgdir}/" install
}


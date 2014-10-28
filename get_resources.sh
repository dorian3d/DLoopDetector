# This script download the resources for the DLoopDetector demo application
#

wget http://doriangalvez.com/resources/DLoopDetector/resources.tar.gz
if [ $? -eq 0 ];
then
  tar xfz resources.tar.gz
fi

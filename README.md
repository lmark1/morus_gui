# morus_gui

#### CAN driver installation
[Download CAN drivers](http://robots.mobilerobots.com/wiki/Peak_Systems_CAN-USB_Interface)  
Peak driver installation:  

_make NET=NETDEV_SUPPORT   
_make install_  

Position inside the repository:  
*sudo modprobe pcan*  
*sudo su*  
*./enable_sh*  
*cat proc/pcan*  

Output should be:  
ndev - can0  
btr - 0x0014


#### Importing to Eclipse workspace

[Eclipse documentation - import existing code](https://help.eclipse.org/neon/index.jsp?topic=%2Forg.eclipse.cdt.doc.user%2Fgetting_started%2Fcdt_w_import.htm)  

After importing project:  
Right click on the project -> C/C++ symbols -> Source Location(tab) and add following folders:  
  $(ROOT_FOLDER)/.  
  QtGui  
  QtWidgets  
  /inc  
  /libuavcan  
  /libuavcan/libuavcan_drivers/linux  
  /qt5  

// TODO remove include redundancies

#### Build 

Build with:  
*make moc_files* - Creates all necessary moc_*.cpp files   
*make*           - Makes all object files and executable in ../build/lmark_gui

Clean with:  
_make clean_ - Remove all object files  
*make clean_moc* - Remove all generated moc_* files in ../src/  


#### Misc
By the way, one extremely important thing to note here is that you should NEVER allocate heap objects (using new) in the constructor of the QObject class as this allocation is then performed on the main thread and not on the new QThread instance, meaning that the newly created object is then owned by the main thread and not the QThread instance. This will make your code fail to work. Instead, allocate such resources in the main function slot such as process() in this case as when that is called the object will be on the new thread instance and thus it will own the resource.




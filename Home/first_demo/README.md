
### This program mainly talked about the process of how to open a JPG and then write into the TIF. 

The main code is saved in the **first_demo.cpp**, and the config files consists the head file **gdal_priv.h**, the source file **gdal_i.lib**, the dynamic link library **gdal18.dll** and the picture **trees.jpg**. you can use these files to run the program on the **visual studio 2017**.

Please remember that there may be some little problems when you run the program.

* **First**: the path's type of the input file and the output file should not be `char *`, I think the better practice is the `const char *`.
* **Second**:  please free the memory at the end of program.

If your result like this:

![](https://i.imgur.com/RBwIgOV.png)


and open the TIF file:

![](https://i.imgur.com/TjJjbJx.jpg)

### **Good Luck!**


## This program mainly talked about the linear filtering of the picture

The code is saved in the **fourth_demo.cpp**. The object of the program is **lena.jpg**. What's more, the result is saved in the **res5\_1.tif**, **res5\_2.tif**, **res5\_3.tif**, **res5\_4.tif**, **res5\_5.tif**. 

$$
	\begin{bmatrix}
	0 & 0.2 & 0 \\\
	0.2 & 0.2 & 0.2 \\\
	0 & 0.2 & 0 \\\ 
	\end{bmatrix} 
	\tag{1}
$$

$$
	\begin{bmatrix}
	0.2 & 0 & 0 & 0 & 0 \\\
	0 & 0.2 & 0 & 0 & 0 \\\
	0 & 0 & 0.2 & 0 & 0 \\\
	0 & 0 & 0 & 0.2 & 0 \\\
	0 & 0 & 0 & 0 & 0.2 \\\
	\end{bmatrix} 
	\tag{2}
$$

$$
	\begin{bmatrix}
	-1 & -1 & -1 \\\
	-1 & 8 & -1 \\\
	-1 & -1 & -1 \\\
	\end{bmatrix}
	\tag{3}
$$

$$
	\begin{bmatrix}
	-1 & -1 & -1 \\\
	-1 & 9 & -1 \\\
	-1 & -1 & -1 \\\
	\end{bmatrix}
	\tag{4}
$$

$$
	\begin{bmatrix}
	-1 & -1 & 0 \\\
	-1 & 0 & 1 \\\
	0  & 1 & 1 \\\
	\end{bmatrix}
	\tag{5}
$$

$$
	\begin{bmatrix}
	0.0120 & 0.1253 & 0.2736 & 0.1253 & 0.0120 \\\
	0.1253 & 1.3054 & 2.8514 & 1.3054 & 0.1253 \\\
	0.2736 & 2.8514 & 6.2279 & 2.8514 & 0.2736 \\\
	0.1253 & 1.3054 & 2.8514 & 1.3054 & 0.1253 \\\
	0.0120 & 0.1253 & 0.2736 & 0.1253 & 0.0120 \\\
	\end{bmatrix}
	\tag{6}
$$

you can use these files to run the program on the **visual studio 2017**. 
### First of all, we must clear and definite the noun "**Linear Filtering**"

# matrix



## **主要功能**：

* 测试几种不同的方法计算矩阵乘法的时间，从中寻求最高效的矩阵计算方法。
* 支持用户输入进行任意大小的矩阵乘法运算，且在遇到异常输入时能够检测出错误，主动终止程序。



## 包含文件：

* 本项目共包含两个源代码文件 matrix1.cpp`和`matrix2.cpp。
* matrix1.cpp 用于测试两个10000*10000的矩阵相乘所需要的时间，用到的方法有分块、换序和openblas。
* matrix2.cpp用于用户的实际使用，当遇到用户输入错误变量或者输入矩阵大小不当时，会分别报出错误。



## 程序1：

* #### 创建矩阵和生成数据：

* 创建用于表示矩阵的结构m：

  ```c++
  struct m {
  	int r;
  	int c;
  	float** a;
  };
  ```

* 定义两个大小为10000*10000的矩阵m1，m2，并用随机函数为它们赋值：

  ```C++
  m m1, m2, m3;
  	m1.r = 10000;
  	m1.c = 10000;
  	m2.r = 10000;
  	m2.c = 10000;
  	m1.a = new float* [m1.r];
  	for (int i = 0; i < m1.r; i++) {
  		m1.a[i] = new float[m1.c];
  	}
  	for (int i = 0; i < m1.r; i++) {
  		for (int j = 0; j < m1.c; j++) {
  			m1.a[i][j] = rand() % 100000 / 100000 + rand() % 100;
  		}
  	}
  	m2.a = new float* [m2.r];
  	for (int i = 0; i < m2.r; i++) {
  		m2.a[i] = new float[m2.c];
  	}
  	for (int i = 0; i < m2.r; i++) {
  		for (int j = 0; j < m2.c; j++) {
  			m2.a[i][j] = rand() % 100000 / 100000 + rand() % 100;
  		}
  	}
  ```

  

* 定义矩阵m3用于储存计算结果：

  ```c++
  m3.r = m1.r;
  	m3.c = m2.c;
  	m3.a = new float* [m3.r];
  	for (int i = 0; i < m3.r; i++) {
  		m3.a[i] = new float[m3.c];
  	}
  ```

* #### 测试时间：

* 初始逐个元素计算的方法：

  ```
  auto start = std::chrono::steady_clock::now();
  	for (int i = 0; i < m3.r; i++) {
  		for (int j = 0; j < m3.c; j++) {
  			m3.a[i][j] = 0;
  			for (int k = 0; k < m1.c; k++) {
  				m3.a[i][j] += m1.a[i][k] * m2.a[k][j];
  			}
  		}
  	}
  	cout << m3.a[0][0] << endl;
  	auto end = std::chrono::steady_clock::now();
  	cout << "use time "
  		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
  		<< " ms" << endl;
  ```

* 分块计算法，每次循环计算八组元素的乘积：

  ```c++
  start = std::chrono::steady_clock::now();
  	for (int i = 0; i < m3.r; i++) {
  		for (int j = 0; j < m3.c; j++) {
  			for (int k = 0; k < m1.c; k += 8) {
  				m3.a[i][j] += m1.a[i][k] * m2.a[k][j];
  				m3.a[i][j] += m1.a[i][k + 1] * m2.a[k + 1][j];
  				m3.a[i][j] += m1.a[i][k + 2] * m2.a[k + 2][j];
  				m3.a[i][j] += m1.a[i][k + 3] * m2.a[k + 3][j];
  				m3.a[i][j] += m1.a[i][k + 4] * m2.a[k + 4][j];
  				m3.a[i][j] += m1.a[i][k + 5] * m2.a[k + 5][j];
  				m3.a[i][j] += m1.a[i][k + 6] * m2.a[k + 6][j];
  				m3.a[i][j] += m1.a[i][k + 7] * m2.a[k + 7][j];
  			}
  		}
  	}
  	cout << m3.a[0][0] << endl;
  	end = std::chrono::steady_clock::now();
  	cout << "use time "
  		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
          << " ms" << endl;
  ```

* 换序计算法，调换计算顺序，改为固定m1中的一个元素，逐次计算完和它相关的乘法，再移动到下一个元素：

  ```c++
  for (int i = 0; i < m3.r; i++) {
  		for (int j = 0; j < m3.c; j++) {
  			m3.a[i][j] = 0;
  		}
  	}
  	start = std::chrono::steady_clock::now();
  	for (int i = 0; i < m3.r; i++) {
  		for (int k = 0; k < m1.c; k++) {
  			float s = m1.a[i][k];
  			for (int j = 0; j < m3.c; j++) {
  				m3.a[i][j] += s * m2.a[k][j];
  			}
  		}
  	}
  	cout << m3.a[0][0] << endl;
  	end = std::chrono::steady_clock::now();
  	cout << "use time "
  		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
  		<< " ms" << endl;
  ```

* 换序加分块，两种方法并用，每次固定m1中四个元素，然后进行16次乘法计算：

  ```c++
  for (int i = 0; i < m3.r; i++) {
  		for (int j = 0; j < m3.c; j++) {
  			m3.a[i][j] = 0;
  		}
  	}
  	start = std::chrono::steady_clock::now();
  	for (int i = 0; i < m3.r; i++) {
  		for (int k = 0; k < m1.c; k += 4) {
  			float s1 = m1.a[i][k];
  			float s2 = m1.a[i][k+1];
  			float s3 = m1.a[i][k+2];
  			float s4 = m1.a[i][k+3];
  			for (int j = 0; j < m3.c; j += 4) {
  				m3.a[i][j] += s1 * m2.a[k][j];
  				m3.a[i][j + 1] += s1 * m2.a[k][j + 1];
  				m3.a[i][j + 2] += s1 * m2.a[k][j + 2];
  				m3.a[i][j + 3] += s1 * m2.a[k][j + 3];
  
  				m3.a[i][j] += s2 * m2.a[k + 1][j];
  				m3.a[i][j + 1] += s2 * m2.a[k + 1][j + 1];
  				m3.a[i][j + 2] += s2 * m2.a[k + 1][j + 2];
  				m3.a[i][j + 3] += s2 * m2.a[k + 1][j + 3];
  				
  				m3.a[i][j] += s3 * m2.a[k + 2][j];
  				m3.a[i][j + 1] += s3 * m2.a[k + 2][j + 1];
  				m3.a[i][j + 2] += s3 * m2.a[k + 2][j + 2];
  				m3.a[i][j + 3] += s3 * m2.a[k + 2][j + 3];
  				
  				m3.a[i][j] += s4 * m2.a[k + 3][j];
  				m3.a[i][j + 1] += s4 * m2.a[k + 3][j + 1];
  				m3.a[i][j + 2] += s4 * m2.a[k + 3][j + 2];
  				m3.a[i][j + 3] += s4 * m2.a[k + 3][j + 3];
  			}
  		}
  	}
  	cout << m3.a[0][0] << endl;
  	end = std::chrono::steady_clock::now();
  	cout << "use time "
  		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
  		<< " ms" << endl;
  ```

* 用openblas计算，因为openblas中的“cblas_sgemm（）”函数用于计算一维数组形式的矩阵，所以先用动态数组读取m1、m2中的元素，再用函数进行计算:

  ```c++
      float* v1 = new float[m1.r * m1.c];
  	float* v2 = new float[m2.r * m2.c];
  	float* v3 = new float[m3.r * m3.c];
  	for (int i = 0; i < m1.r; i++) {
  		for (int j = 0; j < m1.c; j++) {
  			v1[i * m1.c + j] = m1.a[i][j];
  		}
  	}
  	for (int i = 0; i < m2.r; i++) {
  		for (int j = 0; j < m2.c; j++) {
  			v2[i * m2.c + j] = m2.a[i][j];
  		}
  	}
  	start = std::chrono::steady_clock::now();
  	cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.r, m2.c, m1.c, 1, v1, m1.c, v2, m2.c, 0, v3, m3.c);
  	cout << v3[0] << endl;
  	end = std::chrono::steady_clock::now();
  	cout << "use time "
  		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
          << " ms" << endl;
  ```

* #### 输出结果：

* 在测试完每种方法的时间后，都会输出计算得出的矩阵的一行一列，以及计算的时间（ms），前者用于验证几种不同方法计算结果是否相同：

  ```c++
  cout << m3.a[0][0] << endl;
  	end = std::chrono::steady_clock::now();
  	cout << "use time "
  		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
  		<< " ms" << endl;
  ```

* 由于运行一亿个元素的矩阵所需时间过长，所以先测试运行1000*1000的矩阵比较运行时间，测试结果如图所示：

  ![m11](https://github.com/taoge-wjcs/matrix/blob/main/picture/m11.png)

* 可以看出，分块加换序的方法能比不优化加速三倍左右，而openblas的速度远快于前面几种，由此选择这两种方法进行10000*10000大小的矩阵的测试，结果如图：

  ![m12](https://github.com/taoge-wjcs/matrix/blob/main/picture/m12.png)

<img src="https://github.com/taoge-wjcs/matrix/blob/main/picture/m13.png" alt="m13" style="zoom: 80%;" />



## 程序2：

* 创建结构m：

  ```c++
  struct m {
  	int r;
  	int c;
  	float **a;
  };
  ```

* 用户输入两个矩阵的大小，如果第一个矩阵的列数不等于第二个矩阵的行数，则程序会报错：

  ```c++
  	m m1,m2;
  	cout<<"enter the row of m1:";
  	cin>>m1.r;
  	cout<<"enter the column of m1:";
  	cin>>m1.c;
  	cout<<"enter the row of m2:";
  	cin>>m2.r;
  	cout<<"enter the column of m2:";
  	cin>>m2.c;
  	if(m1.c!=m2.r){
  		cout<<"these two matrices can not multiply!";
  		return 0;
  	}
  ```

* 用户输入两个矩阵的元素，在输入过程中，程序会检测输入的是否为浮点数，如果检测到不当输入会报错并终止程序：

  ```c++
  cout<<"enter m1:"<<endl;
  	m1.a=new float*[m1.r];
  	for(int i=0;i<m1.r;i++){
  		m1.a[i]=new float[m1.c];
  	}
  	for(int i=0;i<m1.r;i++){
  		for(int j=0;j<m1.c;j++){
  			cin>>m1.a[i][j];
  			if(cin.fail()){
  				cout<<"Please enter a right matrix!";
  				return 0;
  			}
  		}
  	}
  	
  	cout<<"enter m2:"<<endl;
  	m2.a=new float*[m2.r];
  	for(int i=0;i<m2.r;i++){
  		m2.a[i]=new float[m2.c];
  	}
  	for(int i=0;i<m2.r;i++){
  		for(int j=0;j<m2.c;j++){
  			cin>>m2.a[i][j];
  			if(cin.fail()){
  				cout<<"Please enter a right matrix!";
  				return 0;
  			}
  		}
  	}
  ```

* 定义矩阵m3用于储存并输出计算结果，由于程序1中的测试结果，计算时采用比较快速且代码简洁的换序方法：

  ```c++
  m m3;
  	m3.r=m1.r;
  	m3.c=m2.c;
  	m3.a=new float*[m3.r];
  	for(int i=0;i<m3.r;i++){
  		m3.a[i]=new float[m3.c];
  	}
  	for (int i = 0; i < m3.r; i++) {
  		for (int j = 0; j < m3.c; j++) {
  			m3.a[i][j] = 0;
  		}
  	}
  	cout<<"The product is:"<<endl;
  	for (int i = 0; i < m3.r; i++) {
  		for (int k = 0; k < m1.c; k++) {
  			float s = m1.a[i][k];
  			for (int j = 0; j < m3.c; j++) {
  				m3.a[i][j] += s * m2.a[k][j];
  			}
  		}
  	}
  	for(int i=0;i<m3.r;i++){
  		for(int j=0;j<m3.c;j++){
  			cout<<m3.a[i][j]<<" ";
  		}
  		cout<<endl;
  	}
  ```

* 正确的输出结果如图所示：

![m21](https://github.com/taoge-wjcs/matrix/blob/main/picture/m21.png)

* 错误输入时的输出结果：

  ![m22](https://github.com/taoge-wjcs/matrix/blob/main/picture/m22.png)

![m23](https://github.com/taoge-wjcs/matrix/blob/main/picture/m23.png)

## 总结：

* 本次项目最终采用了优化效率最佳的方法，分块换序法，而得出的结果相比于openblas还是差了十倍以上，更加高效的计算方法还有待考察。

  

## 感谢阅读！

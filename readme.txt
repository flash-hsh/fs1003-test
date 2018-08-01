
（1）硬件抽象层（HAL） Hardware Abstract Layer

　　实现对片内资源 (如定时器、ADC、中断、I/O等) 的通用配置，隐藏具体的SFR操作细节，为上层提供简单清晰的调用接口。

（2）功能模块层（FML）Functional Module Layer

　　通过调用 HAL，实现项目中所涉及到的各片外功能模块，隐藏具体的模块操作细节，并为上层提供简单清晰的调用接口。

（3）应用程序层（APL）Application Layer

　　通过调用 HAL 与 FML，实现最终的应用功能。			
					


状态机参考链接：
https://www.cnblogs.com/skyfsm/p/7071386.html
					
					
					
					
					
					

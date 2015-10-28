2-bit branch prediction using PIN
=========================================

### compile
		cd {path to MyPinTool}
		make MyPinTool.test TARGET=intel64

### run (using tar to compress the lab folder as a test case)
        ../../../pin  -t obj-intel64/MyPinTool.dylib  --  $HOME/Desktop/tar  -cf $HOME/Desktop/lab.tar $HOME/Desktop/lab
        
### result

Total Branches: 1520765 <br/>
Branches that predicted correctly: 1441299 <br/>
Correct rate : 94.775%  
        
				

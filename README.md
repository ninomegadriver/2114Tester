# 2114 (1024 x 4bits) RAM Tester
Arduino sketch for testing 2114 (1024 x 4bits) RAM chips  
Nino MegaDriver - nino@nino.com.br  
  
Update pin assignment to your needs, upload to a 5v Arduino with enough pins and use the serial console to send commands.  
  
Tested using many different used and reciclyed chips and also brand new NEC ones. As always, test more than once to be sure, either if the tests report good or bad. It's quite usual for any sort of even small corrosion make values to float abnormally.  
  
![Picture of 2114 chip being tested](https://raw.githubusercontent.com/ninomegadriver/2114Tester/main/2114Tester.jpg)  
  
Serial console test and output sample:
```
********************************************************
2114 Tester ready...                                    
                                                        
Input:                                                  
testlow    => test with 0s                              
testhigh   => test with 1s                              
bruteforce => test all combination of bits possible     
default    => test everything three times               
********************************************************
Tests run #0
Testing with fixed state: 0
TEST [0]: OK
Testing with fixed state: 1
TEST [1]: OK
Brute forcing: BRUTE FORCE: OK!
********************************************************
Tests run #1
Testing with fixed state: 0
TEST [0]: OK
Testing with fixed state: 1
TEST [1]: OK
Brute forcing: BRUTE FORCE: OK!
********************************************************
Tests run #2
Testing with fixed state: 0
TEST [0]: OK
Testing with fixed state: 1
TEST [1]: OK
Brute forcing: BRUTE FORCE: OK!
********************************************************
2114 Tester ready...                                    
                                                        
Input:                                                  
testlow    => test with 0s                              
testhigh   => test with 1s                              
bruteforce => test all combination of bits possible     
default    => test everything three times               
********************************************************
```


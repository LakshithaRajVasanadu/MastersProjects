import os
import random
for filename in os.listdir('spam'):
         i=random.randint(1, 3);
         str= "mv spam/%s"%(filename);
         str1=" spam_training/ ";
         str2=" spam_testing/ ";
         if(i%2 == 0 or i%3 == 0):
            str=str+str1;
         else:
            str=str+str2;
         print str;
         os.system(str);

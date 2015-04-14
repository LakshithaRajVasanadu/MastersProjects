import os
print "ham_training";
print os.system("ls -1 ham_training | wc -l");
print "ham_testing";
print os.system("ls -1 ham_testing | wc -l");
print "spam_training";
print os.system("ls -1 spam_training | wc -l");
print "spam_testing";
print os.system("ls -1 spam_testing | wc -l");

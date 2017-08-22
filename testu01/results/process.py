
import re
import sys

import glob
listofnames = sys.argv[1:]
listofnames.sort()
for name in listofnames:
  m = re.findall(r"(?<=Test                          p-value).*(?=All other tests were passed)"," ".join(open(name).readlines()), re.MULTILINE|re.DOTALL)
  if(len(m)>0):
    print("== "+name)
    print("".join(m[0].splitlines(True)[2:-2]))

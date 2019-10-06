# !! SHOULD RUN WITH ADMINISTRATOR PRIVILEGE !!
# 
import pyautogui as pa
import time
import sys

print("Place your cursor on the password field")
for i in range(10, 0, -1):
	print("START AFTER", i, "seconds", end="\r")
	time.sleep(1)

pw_list = [str(x) + str(y) for x in range(10) for y in range(10)]
for pw in pw_list:
	print(pw)
	pa.press('backspace')
	pa.typewrite(pw)
	pa.press('enter')
	pa.press('enter')
	pa.press('enter')
	pa.press('enter')

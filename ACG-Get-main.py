import requests
from time import sleep
#GreenHub激活码生成器 

headers = {"Authorization": "(这里填密钥，自己去获取，本人不直接附赠)"}
result = requests.get("https://api.stripe.com/v1/customers", headers=headers).json()

license_code = [customer['metadata']['license_code'] for customer in result['data']]
teststr =''
for i in license_code:
    teststr = i
    print(teststr)

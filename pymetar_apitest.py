#######################################################
#   
#   Testing API response time
#
#######################################################
import time
import requests

NUM_CALLS = 15
i = 0
log = []
params = {
        'ids': 'KPHX',
        'format': 'raw'
    }

response = ''

print('Beginning API calls...')

while i < NUM_CALLS:
    start = time.time()
    response = requests.get("https://aviationweather.gov/api/data/metar", params=params)
    # print(response)
    response.raise_for_status() # Error checking to be fair to the C implementation
    # print(response.json())
    end = time.time()
    log.append(end-start)
    time.sleep(2)
    i += 1

print('Evaluation complete. Writing results.')

with open('C:/Users/etsel/Desktop/IEE380Project/python_time.txt', 'w') as f:
    for line in log:
        f.write("%s\n" % line)

print('Task completed successfully!')

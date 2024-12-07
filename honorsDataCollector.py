#######################################################
#   
#   Testing API response time
#
#######################################################
import time
import pycurl
import certifi
from io import BytesIO # We're gonna have to do some low-level stuff

# Credit goes to
# - https://stackoverflow.com/questions/5345599/how-to-set-get-parameters-with-pycurl

NUM_CALLS = 16
i = 0
log = []
params = {
    'ids': 'KPHX',
    'format': 'raw'
    }
buffer = BytesIO()
c = pycurl.Curl() # Note the similarities to the c-version
c.setopt(c.URL, 'https://aviationweather.gov/api/data/metar?ids=KPHX&format=raw') # I have to manually encode the params in my URL
c.setopt(c.WRITEDATA, buffer)

print('Beginning API calls...')

while i < NUM_CALLS:
    start = time.time()
    c.perform()
    # NOTE: THERE IS NO BUILT-IN ERROR CHECKING WITH PYCURL. This is probably going to make this *very* fast.
    end = time.time()
    log.append(end-start)
    time.sleep(2)
    i += 1

c.close()

print('Evaluation complete. Writing results.')

with open('C:/Users/etsel/Desktop/IEE380Project/honors_time.txt', 'w') as f:
    for line in log:
        f.write("%s\n" % line)

print('Task completed successfully!')

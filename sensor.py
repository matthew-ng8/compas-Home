"""Platform for sensor integration."""
import logging #importing the logging fucntion
from homeassistant.const import TEMP_CELSIUS #this is a str variable made in constants class
from homeassistant.helpers.entity import Entity #entity is an abstract class that this is more or less an extension of
from subprocess import Popen, PIPE
import shlex


import pygatt #importing what I think comes with the necessary tools to help me
from pygatt.exceptions import NotConnectedError
import btlewrap
from btlewrap.base import BluetoothInterface

logging.basicConfig(filename='/tmp/bleSensor.log', filemode='w', format='%(name)s - %(levelname)s - %(message)s')
logging.basicConfig(level=logging.DEBUG)
_LOG = logging.getLogger(__name__) #settting up the logger so I can write stuff to the log and see what went wrong
#async def setup_platform(hass, config, add_entities, discovery_info=None):
logging.debug("HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH")
def setup_platform(hass, config, add_entities, discovery_info=None):
    try:
        f = open("myDebugLog.txt","w")
        f.write("I STARTED")
        _LOG.warning("WELL WE GOT INTO CREATING THE FILE")
    except FileNotFoundError:
        _LOG.warning("Unable to open the file!!!!!!!!!!!!!!!")
    connected = False

    """Set up the sensor platform."""
    f.write("TRYING TO SET UP THE BACKEND")
    
    #backend = pygatt.GATTToolBackend()
    #backend.start()
    
    backend = btlewrap.GatttoolBackend()
    f.write(str(backend.is_connected()))

    while(not connected):
        try:
            device = backend.connect('DC:55:15:8C:A5:50')#same here, try to do like miFlora
            connected = backend.is_connected()
            f = open("myDebugLog.txt","a+")
            f.write("Connected to the BLE sensor /n")
            f.write(str(backend.is_connected()))
            f.write("\n")
            f.close()
            #backend.stop()
        except NotConnectedError as e:
            f = open("myDebugLog.txt","a+")
            f.write("WELP COULDN'/t connect to stuff for BLE /n")
            f.close()
            connected = False
    #eventually we can try to do it like Mi FLora
    #where she scans and stuff and adds each one at a time or rather makes a list and polls them all
    #value = device.char_read("19B10010-E8F2-537E-4F6C-D104768A1909")
    #_LOGGER.debug(value)
    
    add_entities([nanoBLE(backend)])#creating an instance of testStuff and adding it to the entities
    #add_entities([nanoBLE()])#creating one without the device to test overloading consturcotr
    f.close()

class nanoBLE(Entity):
    """Representation of a Sensor."""

    def __init__(self, d):
        """Initialize the sensor."""
        f = open("/tmp/myDebugLog1.txt","w")
        self._state = None
        self._device = d
        self._connected = False
        f.write("created an instance of the sensor")
        f.close()

    @property
    def name(self):
        """Return the name of the sensor."""
        return 'bleSensor'

    @property
    def state(self):
        """Return the state of the sensor."""
        return self._state

    @property
    def unit_of_measurement(self):
        """Return the unit of measurement."""
        return TEMP_CELSIUS
    def update(self):
        """Fetch new state data for the sensor.
        This is the only method that should fetch new data for Home Assistant.
        """
        f = open("/tmp/myDebugLog1.txt","a+")
        #self._state = self._device.read_handle(0x00C)
        #f.write(self._state.decode("utf-8"))
        #f.write(str(self._device.is_connected()))
        f.write("attempting to get data")
        f.write("\n")

        command = "gatttool --device=DC:55:15:8C:A5:50 --char-read -a 0x00C --adapter=hci0"
        args = shlex.split(command)
        for x in args:
            f.write(x+ '\n')
        data= Popen(args, stdin= PIPE, stdout=PIPE, stderr=PIPE).communicate()[0]
        data = data.decode("utf-8")
        f.write(data)
        data = shlex.split(data)
        for x in data:
            f.write(x)
        self._state = int(data[2],16)
        

        f.close()

# publisher
import paho.mqtt.client as mqtt

client = mqtt.Client()
client.connect('0.0.0.0', 1883)

while True:
    client.publish("VR_CONTEXT", input('Message : '))
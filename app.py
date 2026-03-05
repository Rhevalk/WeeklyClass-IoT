from flask import Flask, render_template, request, jsonify
import paho.mqtt.client as mqtt
import json

app = Flask(__name__)

# Config
MQTT_BROKER = "broker.hivemq.com"
MQTT_PORT = 1883

KEY = "A7x9K2mQ"
TOPIC_SENSOR = f"{KEY}/sensor/data"
TOPIC_ALERT = f"{KEY}/alert/gas"

latest_data = {
    "temperature": 0,
    "humidity": 0,
    "gas": 0,
    "alert": "-"
}

def on_connect(client, userdata, flags, reason_code, properties):
    print("Connected to MQTT:", reason_code)
    client.subscribe(TOPIC_SENSOR)
    client.subscribe(TOPIC_ALERT)

def on_message(client, userdata, msg):
    global latest_data

    try:
        if msg.topic == TOPIC_SENSOR:
            payload_str = msg.payload.decode().strip()

            if not payload_str:
                return

            payload = json.loads(payload_str)

            latest_data["temperature"] = payload.get("temperature", 0)
            latest_data["humidity"] = payload.get("humidity", 0)
            latest_data["gas"] = payload.get("gas", 0)

        elif msg.topic == TOPIC_ALERT:
            latest_data["alert"] = msg.payload.decode()

    except Exception as e:
        print("MQTT Parse Error:", e)

mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

mqtt_client.connect(MQTT_BROKER, MQTT_PORT, 60)
mqtt_client.loop_start()

# Endpoint
@app.route("/")
def index():
    return render_template("index.html")

@app.route("/api/data", methods=["GET"])
def get_data():
    return jsonify(latest_data)

@app.route("/api/led", methods=["POST"])
def control_led():
    data = request.json
    led = data["led"]
    state = data["state"]

    topic = f"{KEY}/dashboard/{led}"

    print(f"led: {led}, state: {state}")
    mqtt_client.publish(topic, state)

    return jsonify({"status": "success"})

# Main
if __name__ == "__main__":
    app.run(debug=True, use_reloader=False)

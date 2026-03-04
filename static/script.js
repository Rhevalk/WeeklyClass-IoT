    // Fetch Data
    function fetchData() {
        fetch("/api/data")
        .then(response => response.json())
        .then(data => {
            document.getElementById("temp").innerText = data.temperature;
            document.getElementById("hum").innerText = data.humidity;
            document.getElementById("gas").innerText = Math.round(data.gas);
            document.getElementById("alert").innerText = data.alert;
        });
    }

    setInterval(fetchData, 2000);
    fetchData();

    // LED Send
    function sendLED(led, state) {
        fetch("/api/led", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                led: led,
                state: state
            })
        });
    }


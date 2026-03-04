IoT Dashboard Gas Alarm – ESP32 + Flask + MQTT

## Requirements

Pastikan sudah terinstall:

* Python 3.x
* pip

Jika belum menginstall Python, download melalui:
[https://www.python.org/downloads/](https://www.python.org/downloads/)

---

## Instalasi Backend (Flask)

### 1. Cek Python dan pip

```bash
python --version
pip --version
```

Pastikan keduanya sudah terdeteksi di terminal.

---

### 2. Membuat Virtual Environment

Disarankan menggunakan virtual environment agar dependency terisolasi.

```bash
python -m venv env
source env/bin/activate   # Linux / Mac
```

Untuk Windows:

```bash
env\Scripts\activate
```

Untuk menonaktifkan environment:

```bash
deactivate
```

---

### 3. Install Dependencies

Menggunakan file `requirements.txt`:

```bash
pip install -r requirements.txt
```

Atau install manual:

```bash
pip install flask paho-mqtt
```

---

## Struktur Project (Contoh)

Folder `/sketch` berisi file `.ino` untuk ESP32. Untuk file ESP32 yang menggunakan mqtt ada di folder `/sketch/esp32_mqtt`.
Peletakan folder `/sketch` tidak wajib berdampingan dengan `app.py`, namun disarankan agar struktur project tetap rapi dan terorganisir.

---

## Cara Menjalankan Server

Dalam folder project, lalu jalankan:

```bash
python app.py
```

Jika berhasil, akan muncul URL seperti:

```
http://127.0.0.1:5000
```

---

## Akses Dashboard

Buka browser dan kunjungi:

```
http://127.0.0.1:5000
```

Atau gunakan URL yang muncul pada terminal setelah server dijalankan.

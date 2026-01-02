from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import subprocess
import requests

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

OLLAMA_PATH = "/opt/homebrew/bin/ollama" 
MODEL_NAME = "phi3"
ROBOT_URL = "http://192.168.4.1"

ALLOWED_COMMANDS = {
    "FORWARD", "BACKWARD", "TURN_LEFT", "TURN_RIGHT", "STOP", "NO_OP"
}

class VoiceRequest(BaseModel):
    transcript: str

class Response(BaseModel):
    command: str
    sent: bool

def run_llm(transcript: str) -> str:
    prompt = f"""
Output exactly ONE command from:
FORWARD
BACKWARD
TURN_LEFT
TURN_RIGHT
STOP
NO_OP

Transcript:
\"\"\"{transcript}\"\"\"

Output:
"""
    result = subprocess.run(
        [OLLAMA_PATH, "run", MODEL_NAME],
        input=prompt,
        text=True,
        capture_output=True
    )
    return result.stdout.strip().upper()

def send_command(cmd: str) -> bool:
    if cmd == "NO_OP":
        return False
    try:
        r = requests.get(f"{ROBOT_URL}/?cmd={cmd}", timeout=1)
        return r.status_code == 200
    except Exception:
        return False

@app.post("/interpret", response_model=Response)
def interpret(req: VoiceRequest):
    command = run_llm(req.transcript)

    if command not in ALLOWED_COMMANDS:
        command = "NO_OP"

    sent = send_command(command)

    return {
        "command": command,
        "sent": sent
    }

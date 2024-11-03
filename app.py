from flask import Flask, render_template, request, jsonify
import subprocess

app = Flask(__name__)

# rota para exibir a página inicial
@app.route('/')
def index():
    return render_template('index.html')

# rota para gerar a chave pública
@app.route('/generate_key', methods=['POST'])
def generate_key():
    result = subprocess.run(["./output/main"], input="1\n", capture_output=True, text=True)
    return jsonify(output=result.stdout)

# rota para criptografar a mensagem
@app.route('/encrypt', methods=['POST'])
def encrypt():
    message = request.form.get("message")
    result = subprocess.run(["./output/main"], input=f"2\n{message}\n", capture_output=True, text=True)
    return jsonify(output=result.stdout)

# rota para descriptografar a mensagem
@app.route('/decrypt', methods=['POST'])
def decrypt():
    result = subprocess.run(["./output/main"], input="3\n", capture_output=True, text=True)
    return jsonify(output=result.stdout)

# rota para exibir a mensagem descriptografada
@app.route('/view_message', methods=['POST'])
def view_message():
    result = subprocess.run(["./output/main"], input="4\n", capture_output=True, text=True)
    return jsonify(output=result.stdout)

if __name__ == '__main__':
    app.run(debug=True)

import { useState } from 'react';

export default function Home() {
  const [file, setFile] = useState<File | null>(null);
  const [message, setMessage] = useState('');
  const [password, setPassword] = useState('');

  const handleUpload = async () => {
    if (!file) {
      setMessage('Bitte wähle eine Datei aus.');
      return;
    }

    const formData = new FormData();
    formData.append('file', file);

    setMessage('Lade hoch…');

    const res = await fetch('/api/upload', {
      method: 'POST',
      headers: {
        Authorization: 'Basic ' + btoa('user:' + password),
      },
      body: formData,
    });

    const data = await res.json();

    if (res.ok) {
      setMessage('Erfolgreich hochgeladen!');
    } else {
      setMessage('Fehler: ' + (data.error || 'Unbekannt'));
    }
  };

  return (
    <main style={{ fontFamily: 'sans-serif', maxWidth: 400, margin: '2rem auto' }}>
      <h1>Bild hochladen</h1>
      <input type="file" onChange={(e) => setFile(e.target.files?.[0] || null)} />
      <br /><br />
      <input
        type="password"
        placeholder="Passwort"
        value={password}
        onChange={(e) => setPassword(e.target.value)}
      />
      <br /><br />
      <button onClick={handleUpload}>Hochladen</button>
      <p>{message}</p>
    </main>
  );
}

import { NextApiRequest, NextApiResponse } from 'next';
import sharp from 'sharp';

export default async function handler(req: NextApiRequest, res: NextApiResponse) {
  try {
    const cloudName = process.env.CLOUDINARY_CLOUD_NAME;
    const imageUrl = `https://res.cloudinary.com/${cloudName}/image/upload/latest.bmp`;

    const response = await fetch(imageUrl);

    if (!response.ok) {
      console.error('Fehler beim Abrufen:', response.statusText);
      return res.status(500).json({ error: 'Bild konnte nicht geladen werden' });
    }

    const arrayBuffer = await response.arrayBuffer();
    const buffer = Buffer.from(arrayBuffer);

    const bwBuffer = await sharp(buffer)
      .resize(800, 480)
      .threshold(128)
      .raw()
      .toBuffer();

    res.setHeader('Content-Type', 'application/octet-stream');
    res.send(bwBuffer);
  } catch (err) {
    console.error('Bildverarbeitung fehlgeschlagen:', err);
    res.status(500).json({ error: 'Fehler beim Umwandeln des Bildes' });
  }
}

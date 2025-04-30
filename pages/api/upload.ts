import { protectWithBasicAuth } from '@/lib/auth';
import { IncomingForm } from 'formidable';
import { v2 as cloudinary } from 'cloudinary';
import type { NextApiRequest, NextApiResponse } from 'next';

export const config = {
  api: {
    bodyParser: false,
  },
};

cloudinary.config({
  cloud_name: process.env.CLOUDINARY_CLOUD_NAME!,
  api_key: process.env.CLOUDINARY_API_KEY!,
  api_secret: process.env.CLOUDINARY_API_SECRET!,
});

export default async function handler(req: NextApiRequest, res: NextApiResponse) {
  if (req.method !== 'POST') return res.status(405).end('Method not allowed');

  if (!protectWithBasicAuth(req, res)) return;

  const form = new IncomingForm({ keepExtensions: true });

  form.parse(req, async (err, fields, files) => {
    if (err || !files.file) {
      console.error('Fehler beim Parsen:', err || 'Keine Datei');
      return res.status(500).json({ error: 'Formular konnte nicht verarbeitet werden' });
    }

    const file = Array.isArray(files.file) ? files.file[0] : files.file;

    try {
      const result = await cloudinary.uploader.upload(file.filepath, {
        public_id: 'latest',
        overwrite: true,
        invalidate: true,
        format: 'bmp',
      });

      console.log('Upload erfolgreich:', result.secure_url);
      res.status(200).json({ url: `https://res.cloudinary.com/${cloudinary.config().cloud_name}/image/upload/latest.bmp` });
    } catch (error) {
      console.error('Fehler beim Hochladen:', error);
      res.status(500).json({ error: 'Upload zu Cloudinary fehlgeschlagen' });
    }
  });
}

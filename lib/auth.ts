import { NextApiRequest, NextApiResponse } from 'next';

export function protectWithBasicAuth(req: NextApiRequest, res: NextApiResponse): boolean {
  const auth = req.headers.authorization;
  const expected = process.env.UPLOAD_PASSWORD;

  if (!auth || !auth.startsWith('Basic ') || !expected) {
    res.status(401).setHeader('WWW-Authenticate', 'Basic').end('Auth erforderlich');
    return false;
  }

  const decoded = Buffer.from(auth.split(' ')[1], 'base64').toString();
  const [_, password] = decoded.split(':');

  if (password !== expected) {
    res.status(401).setHeader('WWW-Authenticate', 'Basic').end('Falsches Passwort');
    return false;
  }

  return true;
}

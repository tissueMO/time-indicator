const jstOffset = 9 * 60 * 60 * 1000;

export async function handler(event) {
  return {
    statusCode: 200,
    headers: {
      'Content-Type': 'text/plain',
    },
    body: new Date(Date.now() + jstOffset).toISOString(),
    isBase64Encoded: false,
  };
}

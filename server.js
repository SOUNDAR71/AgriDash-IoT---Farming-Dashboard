const express = require('express');
const app = express();
const port = 3000;

app.use(express.json());

app.post('/data', (req, res) => {
  console.log('📥 Received from ESP:', req.body);
  res.send('✅ Data received');
});

app.listen(port, () => {
  console.log(`🚀 Server running at http://192.168.43.83:${port}`);
});

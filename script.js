// Channel ID e chave de leitura da API do ThingSpeak
const channelID = 2667482;
const readAPIKey = 'HR35XM1M9DHQPHWH';

// URL para buscar os dados do canal
const url = `https://api.thingspeak.com/channels/${channelID}/fields/1.json?api_key=${readAPIKey}&results=1`;

// Função para buscar e atualizar o valor da força G
async function updateGForce() {
    try {
        const response = await fetch(url);
        const data = await response.json();
        
        // Acessa o valor mais recente do campo 1
        const latestEntry = data.feeds[0].field1;
        const gForceElement = document.getElementById('g-force');
        
        // Atualiza o conteúdo da div
        gForceElement.textContent = `Força G: ${latestEntry}`;
    } catch (error) {
        console.error('Erro ao buscar dados:', error);
    }
}

// Atualiza a cada 15 segundos
updateGForce(); // Primeira chamada imediata
setInterval(updateGForce, 15000); // Intervalo de 15 segundos

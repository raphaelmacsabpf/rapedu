README
======

Trabalho de Banco de Dados 2
Por Raphael Machado dos Santos e Eduardo Mendes Senger

Este trabalho utiliza a licença GNU General Public License.

void initBuffer(buffer *bPool,int lenght,field *fieldList, int fieldCount);

Esta função inicializa o buffer colocando os parâmetros standard em todas variáveis que serão utilizadas
  buffer *bPool           Qual Buffer Pool deve ser iniciado
  int lenght              Tamanho do Buffer
  field *fieldList        Onde os campos vão ser armazanados
  int fieldCount          Contagem de quantos campos terão neste BufferPool
  RETORNO                 Nenhum




void findNextAvaliable(buffer *bPool);

Esta função procura a próxima página disponível
  buffer *bPool           Buffer Pool a ser trabalhado
  RETORNO                 NENHUM

void applyReplacementPolicies(buffer *bPool);

Esta função aplica as políticas de substituição, a política utilizada tende a remover a página do buffer que contém o menor pinCount
  buffer *bPool           Buffer Pool a ser trabalhado
  RETORNO                 NENHUM




void bufferInsert(buffer *bPool,char *tuple, int diskSeek, int tupleLenght);

Esta função insere um novo elemento no buffer
  buffer *bPool           Buffer Pool a ser inserido
  char *tuple             String da tupla a ser inserida no Buffer Pool
  int diskSeek            Posição no disco onde essa página se encontra
  int tupleLenght         Tamanho máximo da tupla a ser inserida
  RETORNO                 NENHUM


int fillBuffer(buffer *bufferPool, field *fieldList,char fMeta[], char fData[]);

Esta fuunção que vai ler os arquivos de dados e metadados e preencher o Buffer Pool com eles
  buffer *bufferPool      Buffer Pool onde serão armazenados os dados.
  field fieldList         Estrutura que contém os campos que foram lidos do arquivo de metadados
  char fMeta[]            String contendo o nome do arquivo de metadados
  char fData[]            String contendo o nome do arquivo de dados
  RETORNO                 0 caso o arquivo de metadados não seja encontrado
  RETORNO                 1 caso o arquivo de dados não seja encontrado
  RETORNO                 2 caso não haja nenhum erro


void showBuffer(buffer *bufferPool)

Imprime na tela o conteúdo de todas as páginas do BufferPool

  buffer *bufferPool      Buffer Pool a ser imprimido na tela
  RETORNO                 NENHUM

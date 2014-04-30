//Este trabalho utiliza a licença GNU General Public License.

//Inicializa o buffer
void initBuffer(buffer *bPool,int lenght,field *fieldList, int fieldCount);
//Procura a próxima página disponível
void findNextAvaliable(buffer *bPool);
//Aplica as políticas de troca
void applyReplacementPolicies(buffer *bPool);
//Insere um novo elemento no buffer
void bufferInsert(buffer *bPool,char *tuple, int diskSeek, int tupleLenght);
//Função que vai ler os arquivos de dados e metadados
int fillBuffer(buffer *bufferPool, field *fieldList,char fMeta[], char fData[]);
//Função que mostrará o conteúdo do Buffer Pool na tela
void showBuffer(buffer *bufferPool);

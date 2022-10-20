# ginga-mulsemedia
Repositório contendo o middleware Ginga estendido para dar suporte ao NCL 4.0, que permite apresentação de aplicações multimídia com múltiplos efeitos sensoriais, e à interação multimodal e com múltiplos usuários. O Ginga é compatível com o sistema operacional Ubuntu 18.

Este trabalho é desenvolvido pelos doutores Marina Josué e Fábio Barreto e pela mestra Eyre Montevecchi, orientados pela professora doutora Débora Saade, da Universidade Federal Fluminense. Como base, foi utilizado o projeto desenvolvido pelo Laboratório Telemídia da PUC-Rio, disponível no link: https://github.com/TeleMidia/ginga.

### Para instalação e preparação do ambiente:

1. Faça o download do branch "ncl4". Esse branch contém o middleware Ginga estendido para dar suporte ao NCL 4.0.
   (Caso tenha feito o download da branch utilizando a opção "Download ZIP", renomeie a pasta do projeto para "ginga-mulsemedia".)
2. Acesse a pasta do projeto, "ginga-mulsemidia" pelo terminal.
3. Execute o arquivo de instalação e configuração por meio do comando:

   `$ bash ./genesis.sh`

4. Após a execução do genesis.sh, o Ginga estará instalado e pronto para executar as aplicações especificadas em NCL 4.0.


### Execução de aplicações NCL:

- Comando para executar um documento NCL a partir da pasta do projeto, "ginga-mulsemidia":

   `$ src/ginga caminhoParaArqNCL/documentoNCL.ncl`

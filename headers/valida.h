#ifndef VALIDA_H
#define VALIDA_H

int valida_preco(float* preco);
int valida_nome(const char* nome);
int valida_data(const char* data);
int valida_categoria(const char* categoria);
int valida_descricao(const char* descricao);
int valida_quantidade(int* quantidade);
int valida_telefone(const char* telefone);
int valida_endereco(const char* endereco);
int validar_nome_produto(const char *nome);
int valida_cpf(const char* cpf);

#endif // VALIDA_H
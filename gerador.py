import math
import random
import sys

# Função matemática para descobrir a orientação de 3 pontos
def ccw(A, B, C):
    return (C[1] - A[1]) * (B[0] - A[0]) > (B[1] - A[1]) * (C[0] - A[0])

# Verifica se a reta (A,B) cruza com a reta (C,D)
def intersect(A, B, C, D):
    # Se compartilham um vértice, não é um "cruzamento" ilegal para grafos planares
    if A == C or A == D or B == C or B == D:
        return False
    return ccw(A, C, D) != ccw(B, C, D) and ccw(A, B, C) != ccw(A, B, D)

def gerar_instancia(num_nos, num_grupos, densidade, arquivo_saida):
    if num_grupos > num_nos:
        print("Erro: O número de grupos não pode ser maior que o número de nós.")
        return

    # 1. Gerar pontos aleatórios 2D (simulando a posição dos nós)
    pontos = [(random.uniform(0, 1000), random.uniform(0, 1000)) for _ in range(num_nos)]

    # 2. Criar todas as possíveis ligações ordenadas por distância
    #    (Ligar os mais próximos primeiro ajuda muito a evitar cruzamentos!)
    arestas_possiveis = []
    for i in range(num_nos):
        for j in range(i + 1, num_nos):
            dist = math.hypot(pontos[i][0] - pontos[j][0], pontos[i][1] - pontos[j][1])
            arestas_possiveis.append((dist, i, j))
    
    arestas_possiveis.sort() # Ordena da menor para a maior distância

    arestas_finais = []
    
    # Estrutura Union-Find para garantir que o grafo fique CONEXO (Árvore Geradora Mínima)
    pai = list(range(num_nos))
    def find(i):
        if pai[i] == i: return i
        pai[i] = find(pai[i])
        return pai[i]

    def union(i, j):
        raiz_i = find(i)
        raiz_j = find(j)
        if raiz_i != raiz_j:
            pai[raiz_i] = raiz_j
            return True
        return False

    # 3. Processar arestas
    for dist, u, v in arestas_possiveis:
        # Verifica se essa aresta cruza com alguma aresta que já aceitamos
        cruza = False
        for (eu, ev, _) in arestas_finais:
            if intersect(pontos[u], pontos[v], pontos[eu], pontos[ev]):
                cruza = True
                break
        
        # Se NÃO cruza, é candidata a entrar no nosso grafo planar
        if not cruza:
            # Se a aresta conecta partes isoladas (garante a conexidade)
            if union(u, v):
                # Peso inteiro positivo (1 a 100, pode alterar se quiser)
                arestas_finais.append((u, v, random.randint(1, 100))) 
            else:
                # Se já estão conectados por outro caminho, adicionamos com base na 'densidade'
                # para criar ciclos e não ser apenas uma árvore simples.
                if random.random() < densidade:
                    arestas_finais.append((u, v, random.randint(1, 100)))

    # 4. Atribuição dos Grupos (Garantindo no mínimo 1 por grupo)
    grupos = [-1] * num_nos
    nos_embaralhados = list(range(num_nos))
    random.shuffle(nos_embaralhados)
    
    # Passo A: Um nó para cada grupo
    for g in range(num_grupos):
        grupos[nos_embaralhados[g]] = g
        
    # Passo B: O restante dos nós entra em grupos aleatórios
    for i in range(num_grupos, num_nos):
        grupos[nos_embaralhados[i]] = random.randint(0, num_grupos - 1)

    # 5. Escrever no formato esperado pelo seu código (C++)
    with open(arquivo_saida, 'w') as f:
        # Cabecalho: NumNos NumArestas NumGrupos
        f.write(f"{num_nos} {len(arestas_finais)} {num_grupos}\n")
        
        # Nós e seus grupos
        # Ordenados pelo ID do nó para ficar organizado
        for i in range(num_nos):
            f.write(f"{i} {grupos[i]}\n")
            
        # Arestas
        for u, v, peso in arestas_finais:
            # Salvando como peso.0 (float) para casar perfeitamente com sua estrutura C++
            f.write(f"{u} {v} {float(peso)}\n")
            
    print(f"Sucesso! Instância '{arquivo_saida}' gerada.")
    print(f" -> Nós: {num_nos} | Arestas: {len(arestas_finais)} | Grupos: {num_grupos}")

if __name__ == "__main__":
    # --- VOCÊ PODE ALTERAR OS PARÂMETROS AQUI ---
    
    QTD_NOS = 50           # Quantidade total de vértices
    QTD_GRUPOS = 7         # Quantidade total de grupos 
    DENSIDADE = 0.5        # De 0.0 (Apenas uma Árvore) até 1.0 (Máximo de arestas sem cruzar)
    NOME_ARQUIVO = "instancia_teste.txt"
    
    gerar_instancia(QTD_NOS, QTD_GRUPOS, DENSIDADE, NOME_ARQUIVO)
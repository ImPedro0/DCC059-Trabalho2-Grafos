import os
import math
import random
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

# Função matemática para descobrir a orientação de 3 pontos
def ccw(A, B, C):
    return (C[1] - A[1]) * (B[0] - A[0]) > (B[1] - A[1]) * (C[0] - A[0])

# Verifica se a reta (A,B) cruza com a reta (C,D)
def intersect(A, B, C, D):
    if A == C or A == D or B == C or B == D:
        return False
    return ccw(A, C, D) != ccw(B, C, D) and ccw(A, B, C) != ccw(A, B, D)

def gerar_instancia(num_nos, num_grupos, densidade, arquivo_saida):
    if num_grupos > num_nos:
        print("Erro: O número de grupos não pode ser maior que o número de nós.")
        return

    # 1. Gerar pontos aleatórios 2D
    pontos = [(random.uniform(0, 1000), random.uniform(0, 1000)) for _ in range(num_nos)]

    # 2. Criar todas as possíveis ligações ordenadas por distância
    arestas_possiveis = []
    for i in range(num_nos):
        for j in range(i + 1, num_nos):
            dist = math.hypot(pontos[i][0] - pontos[j][0], pontos[i][1] - pontos[j][1])
            arestas_possiveis.append((dist, i, j))
    arestas_possiveis.sort() 

    arestas_finais = []
    
    # Estrutura Union-Find (Garante conexidade)
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
        cruza = False
        for (eu, ev, _) in arestas_finais:
            if intersect(pontos[u], pontos[v], pontos[eu], pontos[ev]):
                cruza = True
                break
        
        if not cruza:
            if union(u, v):
                arestas_finais.append((u, v, random.randint(1, 100))) 
            else:
                if random.random() < densidade:
                    arestas_finais.append((u, v, random.randint(1, 100)))

    # 4. Atribuição dos Grupos (Mínimo 1 por grupo)
    grupos = [-1] * num_nos
    nos_embaralhados = list(range(num_nos))
    random.shuffle(nos_embaralhados)
    
    for g in range(num_grupos):
        grupos[nos_embaralhados[g]] = g
        
    for i in range(num_grupos, num_nos):
        grupos[nos_embaralhados[i]] = random.randint(0, num_grupos - 1)

    # 5. Escrever no arquivo txt
    with open(arquivo_saida, 'w') as f:
        f.write(f"{num_nos} {len(arestas_finais)} {num_grupos}\n")
        for i in range(num_nos):
            f.write(f"{i} {grupos[i]}\n")
        for u, v, peso in arestas_finais:
            f.write(f"{u} {v} {float(peso)}\n")
            
    print(f"Sucesso! Instância '{arquivo_saida}' gerada.")

    # Salvar as coordenadas para o plotador
    arquivo_coords = arquivo_saida.replace('.txt', '_coords.txt')
    with open(arquivo_coords, 'w') as f:
        for x, y in pontos:
            f.write(f"{x} {y}\n")
            

    # ==========================================
    # 6. GERAR E SALVAR A REPRESENTAÇÃO GRÁFICA
    # ==========================================
    plt.figure(figsize=(10, 8))
    
    # Desenhar as arestas em cinza claro
    for u, v, _ in arestas_finais:
        x_vals = [pontos[u][0], pontos[v][0]]
        y_vals = [pontos[u][1], pontos[v][1]]
        plt.plot(x_vals, y_vals, color='gray', linestyle='-', linewidth=1.0, alpha=0.6, zorder=1)
    
    cmap = plt.get_cmap('tab20')
    
    # Desenhar os nós
    for i in range(num_nos):
        g = grupos[i]
        cor = cmap(g % 20)
        plt.scatter(pontos[i][0], pontos[i][1], color=cor, s=80, edgecolors='black', zorder=5)

    # --- CRIAR A LEGENDA DOS GRUPOS ---
    legend_patches = []
    for g in range(num_grupos):
        cor = cmap(g % 20)
        # Cria um "retângulo de cor" para a legenda
        patch = mpatches.Patch(color=cor, label=f'Grupo {g}')
        legend_patches.append(patch)
        
    # Posiciona a legenda no canto superior direito, do lado de fora do gráfico
    plt.legend(handles=legend_patches, title="Legenda", loc='upper left', bbox_to_anchor=(1.02, 1), borderaxespad=0.)

    # Título com as informações básicas
    plt.title(f"Arquivo: {arquivo_saida}\nNós: {num_nos} | Grupos: {num_grupos} | Densidade: {densidade}", fontsize=14, fontweight='bold', pad=15)
    
    plt.axis('off')
    
    # O bbox_inches='tight' garante que a legenda externa não seja cortada na imagem salva
    nome_imagem = arquivo_saida.replace('.txt', '.png')
    plt.savefig(nome_imagem, bbox_inches='tight', dpi=150)
    plt.close()
    
    print(f"Gráfico representativo salvo como: '{nome_imagem}'")


if __name__ == "__main__":
    import os # Permite criar pastas no sistema
    
    # --- PARÂMETROS ---
    QTD_NOS = 100           
    QTD_GRUPOS = 10         
    DENSIDADE = 0.4        
    
    # Apenas o nome da instância (sem o .txt)
    NOME_INSTANCIA = "grafo3"
    
    if not os.path.exists('instancias'):
        os.makedirs('instancias')
        
    pasta_destino = os.path.join('instancias', NOME_INSTANCIA)
    if not os.path.exists(pasta_destino):
        os.makedirs(pasta_destino)
        
    arquivo_saida = os.path.join(pasta_destino, f"{NOME_INSTANCIA}.txt")
    
    # Roda o gerador salvando tudo dentro da nova pasta
    gerar_instancia(QTD_NOS, QTD_GRUPOS, DENSIDADE, arquivo_saida)
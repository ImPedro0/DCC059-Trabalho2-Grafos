import sys
import os
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

def plotar_solucao(caminho_instancia):
    base_nome = caminho_instancia.rsplit('.', 1)[0]
    
    arquivo_instancia = caminho_instancia
    arquivo_coords = base_nome + '_coords.txt'
    arquivo_solucao = base_nome + '_solucao.txt'
    
    if not os.path.exists(arquivo_instancia) or not os.path.exists(arquivo_coords) or not os.path.exists(arquivo_solucao):
        print("Erro: Arquivos não encontrados. Verifique se passou o caminho correto para a instância.")
        print(f"Buscando: \n - {arquivo_instancia}\n - {arquivo_coords}\n - {arquivo_solucao}")
        return
        
    # LER INSTÂNCIA
    with open(arquivo_instancia, 'r') as f:
        linhas = f.read().strip().split('\n')
    
    num_nos, num_arestas, num_grupos = map(int, linhas[0].split())
    grupos = [-1] * num_nos
    for i in range(1, 1 + num_nos):
        u, g = map(float, linhas[i].split())
        grupos[int(u)] = int(g)
        
    arestas_todas = []
    for i in range(1 + num_nos, len(linhas)):
        u, v, p = linhas[i].split()
        arestas_todas.append((int(u), int(v)))
        
    # LER COORDENADAS
    pontos = []
    with open(arquivo_coords, 'r') as f:
        for linha in f:
            x, y = map(float, linha.split())
            pontos.append((x, y))
            
    # LER SOLUÇÃO
    arestas_solucao = []
    custo_solucao = None
    with open(arquivo_solucao, 'r') as f:
        for linha in f:
            linha = linha.strip()
            if not linha: continue
            if linha.lower().startswith('custo:'):
                custo_solucao = linha.split(':')[1].strip()
                continue
            
            partes = linha.split()
            if len(partes) >= 2:
                u, v = int(partes[0]), int(partes[1])
                arestas_solucao.append((u, v))
            
    # ==========================
    # DESENHAR
    # ==========================
    plt.figure(figsize=(10, 8))
    
    # 1. Arestas do Grafo Base (cinza claro)
    for u, v in arestas_todas:
        x_vals = [pontos[u][0], pontos[v][0]]
        y_vals = [pontos[u][1], pontos[v][1]]
        plt.plot(x_vals, y_vals, color='gray', linestyle='-', linewidth=1.0, alpha=0.3, zorder=1)
        
    # 2. Arestas da Solução (vermelho, mais grosso)
    for u, v in arestas_solucao:
        x_vals = [pontos[u][0], pontos[v][0]]
        y_vals = [pontos[u][1], pontos[v][1]]
        plt.plot(x_vals, y_vals, color='red', linestyle='-', linewidth=3.5, alpha=0.9, zorder=2)
        
    # 3. Nós coloridos por grupo
    cmap = plt.get_cmap('tab20')
    for i in range(num_nos):
        g = grupos[i]
        cor = cmap(g % 20)
        plt.scatter(pontos[i][0], pontos[i][1], color=cor, s=80, edgecolors='black', zorder=5)
        
    # 4. Legenda
    legend_patches = []
    for g in range(num_grupos):
        cor = cmap(g % 20)
        patch = mpatches.Patch(color=cor, label=f'Grupo {g}')
        legend_patches.append(patch)
        
    # Legenda extra para o Caminho Solução
    legend_patches.append(mpatches.Patch(color='red', label='Melhor Solução'))
        
    plt.legend(handles=legend_patches, title="Legenda", loc='upper left', bbox_to_anchor=(1.02, 1), borderaxespad=0.)

    nome_arquivo = os.path.basename(caminho_instancia)
    titulo = f"Solução Encontrada: {nome_arquivo}"
    if custo_solucao:
        titulo += f" (Custo: {custo_solucao})"
    titulo += "\nAs linhas vermelhas representam o melhor caminho construído pelo GRASP."
    plt.title(titulo, fontsize=13, fontweight='bold', pad=15)
    plt.axis('off')
    
    arquivo_imagem = base_nome + '_solucao.png'
    plt.savefig(arquivo_imagem, bbox_inches='tight', dpi=150)
    plt.close()
    
    print(f"Gráfico da solução salvo como: '{arquivo_imagem}'")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: python plotar_solucao.py <caminho_da_instancia.txt>")
    else:
        plotar_solucao(sys.argv[1])

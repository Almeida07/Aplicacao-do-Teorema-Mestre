import pandas as pd, numpy as np, matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
df = pd.read_csv("resultados.csv")
g = {1: lambda n: n**2, 2: lambda n: n*np.log2(n), 3: lambda n: n**2}
gname = {1: "n²", 2: "n log₂ n", 3: "n²"}
out = {}
for p in (1,2,3):
    d = df[df.problema==p]
    a = d.groupby("n").agg(t_med=("tempo_s","median"), t_mean=("tempo_s","mean"), t_std=("tempo_s","std"),
                           ops=("operacoes","mean")).reset_index()
    n = a.n.values.astype(float)
    # ajuste por minimos quadrados no espaco relativo (c que minimiza soma de (t/(c g) - 1)^2)
    G = g[p](n)
    c_t = np.sum(a.t_med/G*1.0 * 1) / len(n)  # media de t/g (constante ajustada)
    c_t = np.sum(a.t_med.values*G)/np.sum(G*G)  # LS padrao
    c_o = np.sum(a.ops.values*G)/np.sum(G*G)
    a["g"] = G
    a["t_teo"] = c_t*G
    a["erro_t"] = (a.t_med - a.t_teo)/a.t_teo*100
    a["ops_teo"] = c_o*G
    a["erro_o"] = (a.ops - a.ops_teo)/a.ops_teo*100
    a["t_sobre_g"] = a.t_med/G
    a["o_sobre_g"] = a.ops/G
    slope_t = np.polyfit(np.log(n), np.log(a.t_med), 1)[0]
    slope_o = np.polyfit(np.log(n), np.log(a.ops), 1)[0]
    out[p] = dict(tab=a, c_t=c_t, c_o=c_o, slope_t=slope_t, slope_o=slope_o)
    fig, ax = plt.subplots(1,2, figsize=(11,4.2))
    ax[0].loglog(n, a.t_med, "o", label="experimental (mediana)")
    ax[0].errorbar(n, a.t_mean, yerr=a.t_std, fmt="none", ecolor="gray", alpha=.6, capsize=2, label="média ± desvio")
    ax[0].loglog(n, a.t_teo, "-", label=f"teórica: c·{gname[p]}")
    ax[0].set_xlabel("n"); ax[0].set_ylabel("tempo (s)"); ax[0].set_title(f"Problema {p}: tempo × n (escala log-log)")
    ax[0].set_xlim(n.min()/1.5, n.max()*1.5); ax[0].grid(True, which="both", alpha=.3); ax[0].legend(fontsize=8)
    ax[1].semilogx(n, a.o_sobre_g/a.o_sobre_g.iloc[-1], "s-", label="operações / g(n) (normalizado)")
    ax[1].semilogx(n, a.t_sobre_g/a.t_sobre_g.iloc[-1], "o--", label="tempo / g(n) (normalizado)")
    ax[1].axhline(1, color="k", lw=.8)
    ax[1].set_xlabel("n"); ax[1].set_ylabel("razão normalizada pelo último ponto")
    ax[1].set_title(f"Problema {p}: razão com g(n) = {gname[p]}")
    ax[1].grid(True, which="both", alpha=.3); ax[1].legend(fontsize=8)
    plt.tight_layout(); plt.savefig(f"fig{p}.png", dpi=150); plt.close()
    a.to_csv(f"tab{p}.csv", index=False)
    print(p, "c_t=%.3e c_o=%.4f slope_t=%.3f slope_o=%.3f" % (c_t, c_o, slope_t, slope_o))
    print(a[["n","t_med","t_mean","t_std","ops","t_teo","erro_t","o_sobre_g","erro_o"]].to_string(index=False))

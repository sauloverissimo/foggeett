
def extract_crypto_fields(ticks, 
                          fields=("price","volume", "high", "low", "quoteVolume"), 
                          passthrough=("tick_id", "timestamp")):
    """
    Extrai campos válidos para indicadores, e permite passar campos extras como tick_id, timestamp.

    - 'fields': exigem float/int válidos e são obrigatórios
    - 'passthrough': opcionais, incluídos se existirem (qualquer tipo)
    """
    resultado = []

    for t in ticks:
        # Confirma que TODOS os campos técnicos são válidos (float/int)
        if not all(k in t and isinstance(t[k], (float, int)) for k in fields):
            continue
        
        tick_filtrado = {
            k: float(t[k]) for k in fields
        }

        for k in passthrough:
            if k in t:
                tick_filtrado[k] = t[k]

        resultado.append(tick_filtrado)

    return resultado
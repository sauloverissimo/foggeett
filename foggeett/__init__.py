# pyright: reportMissingImports=false
# type: ignore

from importlib import metadata as _md
from ._core import slope, sma, wma, enrich, ema, mom, roc, macd, obv, adx, KPIConfig, KPIResult
from .functions.fields import extract_crypto_fields  # <- nova função utilitária

__all__ = [
    "slope", "sma", "wma", "ema", "mom", "roc", "macd", "obv", "adx",
    "enrich", "KPIConfig", "KPIResult",
    "extract_crypto_fields",  # <- incluído no __all__
    "__version__"
]

__version__ = _md.version(__name__)


# from importlib import metadata as _md
# from ._core import slope, sma, wma, enrich, ema, mom, roc, macd, obv, adx, KPIConfig, KPIResult

# __all__ = ["slope", "sma","wma", "ema","mom","roc","macd","obv", "adx", "enrich", "KPIConfig", "KPIResult", "__version__"]
# __version__ = _md.version(__name__)

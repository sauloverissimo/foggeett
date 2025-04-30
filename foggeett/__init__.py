from importlib import metadata as _md
from ._core import slope, sma, wma, enrich, ema, mom, roc, macd, obv, KPIConfig, KPIResult

__all__ = ["slope", "sma","wma", "ema","mom","roc","macd","obv","enrich", "KPIConfig", "KPIResult", "__version__"]
__version__ = _md.version(__name__)

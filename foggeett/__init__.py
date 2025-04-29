from importlib import metadata as _md
from ._core import slope, sma, enrich, KPIConfig, KPIResult

__all__ = ["slope", "sma", "enrich", "KPIConfig", "KPIResult", "__version__"]
__version__ = _md.version(__name__)

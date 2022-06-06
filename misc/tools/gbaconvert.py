# Convert Tiled map to GBA tilemap
#
# Authored by: Rushi

from tiled import *
from pathlib import Path

class GBAConvert(Plugin):
    @classmethod
    def nameFilter(cls):
        return "Header files (*.h)"

    @classmethod
    def shortName(cls):
        return "gbaconvert"

    @classmethod
    def write(cls, tileMap, fileName):
        with open(fileName, 'w') as fileHandle:
            meta = 32

            print("const unsigned short " + Path(fileName).stem + "[] = {\n", file=fileHandle)

            for i in range(tileMap.layerCount()):
                if isTileLayerAt(tileMap, i):
                    tileLayer = tileLayerAt(tileMap, i)
                    for y in range(tileLayer.height()):
                        tiles = []
                        line = ''

                        for x in range(tileLayer.width()):
                            if x % meta == 0:
                                tiles.append('\t')

                            if tileLayer.cellAt(x, y).tile() != None:
                                cell = tileLayer.cellAt(x, y)
                                tile = cell.tile()
                                props = tile.propertyAsString('palette')
                                flip = (int(cell.flippedHorizontally) << 0x0A) | (int(cell.flippedVertically) << 0x0B)
                                palette = 0

                                if props > '0':
                                    palette = (int(props) - 1) << 0x0C

                                tiles.append("0x{0:04x}".format(tile.id() | flip | palette))
                            else:
                                tiles.append(str(-1))

                        if y % meta == 0:
                            line += '// {}\n'.format(meta)

                        line += ','.join(tiles)

                        if y == tileLayer.height():
                            line += '\n};'
                        else:
                            line += ','

                        print(line, file=fileHandle)

        return True

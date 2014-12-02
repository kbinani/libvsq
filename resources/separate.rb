fname = "PhoneticSymbolDictionaryVocaloidEn.inc"
LIMIT = 65000
LIMIT2 = 16000
open(fname, "r") { |file|
  lines = 0
  number = 0
  size = 0
  size2 = 0
  pair = ""
  pair.force_encoding("UTF-8")
  out = open("../src/PhoneticSymbolDictionaryVocaloidEn.#{number}.inc", "w")
  out.write('R"(')
  file.each { |line|
    line.force_encoding("UTF-8")
    lines = lines + 1
    pair = "#{pair}#{line}"
    if lines % 2 == 0
      if size + pair.bytesize > LIMIT
        out.write(')"')
        out.close
        number = number + 1
        out = open("../src/PhoneticSymbolDictionaryVocaloidEn.#{number}.inc", "w")
        out.write('R"(')
        size = 0
        size2 = 0
      end

      if size2 + pair.bytesize > LIMIT2
        out.write(')"')
        out.write("\n")
        out.write('R"(')
        size2 = 0
      end

      out.write(pair)
      size = size + pair.bytesize
      size2 = size2 + pair.bytesize

      pair = ""
      pair.force_encoding("UTF-8")
    end
  }
  out.write(')"')
  out.close
}


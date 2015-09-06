class CompressEnumerable
  include Enumerable

  def initialize(str)
    @comp_str, @index = [], []
      unless str.nil?
        words = str.scan(/\w+/)
        @comp_str = words.uniq
        @index = words.map{ |word| @comp_str.index(word)}
      end
  end

  def each
    strings = Array.new
    for i in 0..index.length-1
      strings << (block_given? ? yield(@comp_str[@index[i]]):@comp_str[@index[i]])
    end      
    strings
  end

  def comp_str
    @comp_str
  end

  def index
    @index
  end
end

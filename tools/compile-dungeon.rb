#!/usr/bin/ruby
# frozen_string_literal: true

# represents a tiled dungeon/world in assembly
# arguments:
# - .world file (input)
# - .s file (output)

require 'bundler/inline'

gemfile do
  source 'https://rubygems.org'

  gem 'nokogiri'
end

require 'json'
require_relative './rle'

class DungeonCompiler
  def initialize(world_file, output_file)
    @world_file = world_file
    @output_file = output_file
    @map_path = File.dirname(world_file)
  end

  def process
    world = JSON.parse(File.read(@world_file))
    File.open(@output_file, 'wb') do |f|
      f.puts '.segment "RODATA"'
      f.puts '.export _starting_room'
      f.puts "_starting_room: .word #{labelify(world['maps'].first['fileName'])}"
      world['maps'].each do |map_info|
        f.puts "#{labelify(map_info['fileName'])}:"
        # pointers for which map is up, down, left, right from here
        up_x = map_info['x']
        up_y = map_info['y'] - map_info['height']
        up_label = world['maps'].find { |m| m['x'] == up_x && m['y'] == up_y }&.then do |data|
          labelify(data['fileName'])
        end || '0'
        f.puts ".word #{up_label}"

        down_x = map_info['x']
        down_y = map_info['y'] + map_info['height']
        down_label = world['maps'].find { |m| m['x'] == down_x && m['y'] == down_y }&.then do |data|
          labelify(data['fileName'])
        end || '0'
        f.puts ".word #{down_label}"

        left_x = map_info['x'] - map_info['width']
        left_y = map_info['y']
        left_label = world['maps'].find { |m| m['x'] == left_x && m['y'] == left_y }&.then do |data|
          labelify(data['fileName'])
        end || '0'
        f.puts ".word #{left_label}"

        right_x = map_info['x'] + map_info['width']
        right_y = map_info['y']
        right_label = world['maps'].find { |m| m['x'] == right_x && m['y'] == right_y }&.then do |data|
          labelify(data['fileName'])
        end || '0'
        f.puts ".word #{right_label}"

        f.puts read_tmx_file(File.join(@map_path, map_info['fileName']))
      end
    end
  end

  private

  def labelify(tmx_name)
    tmx_name.tr('-.', '__')
  end

  def read_tmx_file(tmx_file)
    document = Nokogiri::XML(File.read(tmx_file))

    metatiles = document.xpath('//layer/data')
                        .text
                        .scan(/\d+/)
                        .map { |t| t.to_i - 1 }

    rle_tiles = RLE.rle(metatiles)

    ".byte #{rle_tiles.join(', ')}"
  end
end

world_file, output_file = ARGV

DungeonCompiler.new(world_file, output_file).process

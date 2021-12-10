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
  gem 'pry'
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
      f.puts <<~"PREAMBLE"
             .enum entity_type
                Fire
                Patrol
             .endenum
             .segment "RODATA"
             .include "cutscene.inc"
             .export _starting_room
             _starting_room: .word #{labelify(world['maps'].first['fileName'])}
      PREAMBLE
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

  def numberify(coordinate)
    coordinate.to_f.round.to_i
  end

  def coalesce(coordinate)
    (coordinate + 4) & ~0b111
  end

  def fmt(byte)
    format('$%02x', byte)
  end

  def read_tmx_file(tmx_file)
    document = Nokogiri::XML(File.read(tmx_file))

    cutscene_pointer = '0'

    cutscene_prop = document.xpath('//property').find { |prop| prop['name'] == 'cutscene' }

    if cutscene_prop
      cutscene_pointer = "_#{cutscene_prop['value']}_cutscene"
    end

    entity_payload = []

    objects = document.xpath('//objectgroup/object')

    entity_payload << objects.count

    objects.each do |object|
      entity_payload << "entity_type::#{object['type']}"
      object_x = numberify(object['x']) - 8
      object_y = numberify(object['y']) - 8
      entity_payload << fmt(coalesce(object_x))
      entity_payload << fmt(coalesce(object_y))
        case object['type']
        when 'Patrol'
          points = object.xpath('./polygon').first['points'].split(/\s+/)
          entity_payload << points.count
          points.each do |point|
            px, py = point.split(/,/).map { |pt| numberify(pt) }
            entity_payload << fmt(coalesce(object_x + px))
            entity_payload << fmt(coalesce(object_y + py))
          end
        end
      end

      metatiles = document.xpath('//layer/data')
                          .text
                          .scan(/\d+/)
                          .map { |t| t.to_i - 1 }

      rle_tiles = RLE.rle(metatiles)
      <<~"TMX_DATA"
      .word #{cutscene_pointer}
      .byte #{entity_payload.join(', ')}
      .byte #{rle_tiles.join(', ')}
    TMX_DATA
  end
end

world_file, output_file = ARGV

DungeonCompiler.new(world_file, output_file).process

BEGIN TRANSACTION;

CREATE TABLE IF NOT EXISTS settings (
  id               UUID PRIMARY KEY DEFAULT uuidv7(),
  target_lang_idx  INTEGER NOT NULL,
  name             VARCHAR
);

CREATE TABLE IF NOT EXISTS decks (
  id           UUID PRIMARY KEY DEFAULT uuidv7(),
  name         VARCHAR NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS cards (
  id               UUID PRIMARY KEY DEFAULT uuidv7(),
  deck_id          UUID NOT NULL REFERENCES decks(id),
  created_at       VARCHAR NOT NULL,
  updated_at       VARCHAR,
  reviewed_at      VARCHAR,

  difficulty       DOUBLE NOT NULL,
  stability        DOUBLE NOT NULL,

  front_text       VARCHAR NOT NULL,
  front_norm_text  VARCHAR NOT NULL,
  back_text        VARCHAR NOT NULL,
  back_norm_text   VARCHAR NOT NULL,
  audio_path       VARCHAR NOT NULL,
  image_path       VARCHAR,

  UNIQUE(deck_id, front_text)
);

COMMIT;

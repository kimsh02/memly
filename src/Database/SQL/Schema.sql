BEGIN TRANSACTION;

CREATE TABLE IF NOT EXISTS migrations (
  version    UINTEGER PRIMARY KEY,
  applied_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS settings (
  id               UUID PRIMARY KEY DEFAULT uuidv7(),
  target_lang_idx  UINTEGER NOT NULL,
);

CREATE TABLE IF NOT EXISTS decks (
  id           UUID PRIMARY KEY DEFAULT uuidv7(),
  name         VARCHAR NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS cards (
  id               UUID PRIMARY KEY DEFAULT uuidv7(),
  deck_id          UUID NOT NULL REFERENCES decks(id),
  created_at       TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  updated_at       TIMESTAMP,
  reviewed_at      TIMESTAMP,

  difficulty       DOUBLE NOT NULL,
  stability        DOUBLE NOT NULL,

  front_text       VARCHAR NOT NULL,
  front_norm_text  VARCHAR NOT NULL,
  back_text        VARCHAR NOT NULL,
  back_norm_text   VARCHAR NOT NULL,
  audio_path       VARCHAR,
  image_path       VARCHAR,

  UNIQUE(deck_id, front_text)
);

COMMIT;
